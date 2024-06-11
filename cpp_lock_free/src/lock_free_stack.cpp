#include <atomic>
#include <functional>
#include <memory>
#include <shared_mutex>
#include <stdexcept>
#include <thread>

namespace memory_leak {

template <typename T>
class lock_free_stack
{
    struct node
    {
        std::shared_ptr<T> m_data;
        node* next;
        node(const T& data) : m_data(std::make_shared<T>(data)), next(nullptr)
        {
        }
    };

private:
    std::atomic<node*> header;

public:
    lock_free_stack(const lock_free_stack& obj) = delete;
    lock_free_stack(lock_free_stack&& obj) = delete;

    lock_free_stack() : header(nullptr) {}

    void push(const T& data)
    {
        node* new_node = new node(data);
        new_node->next = header.load();
        while (!header.compare_exchange_weak(new_node->next, new_node))
            ;
    }

    std::shared_ptr<T> pop()
    {
        node* old_head = header.load();
        while (old_head &&
               !header.compare_exchange_weak(old_head, old_head->next))
            ;
        if (old_head == nullptr)
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> res;
        res.swap(old_head->m_data);
        delete old_head;
        return res;
    }
};

} // namespace memory_leak

namespace gc {

template <typename T>
class lock_free_stack
{
private:
    struct node
    {
        std::shared_ptr<T> m_data;
        node* m_next;

        node(const T& data) : m_data(std::make_shared<T>(data)), m_next(nullptr)
        {
        }
    };

    std::atomic<node*> m_header;

public:
    lock_free_stack(const lock_free_stack&) = delete;
    lock_free_stack(lock_free_stack&&) = delete;
    lock_free_stack() : m_header(nullptr) {}

    void push(T data)
    {
        node* new_node = new node(data);
        new_node->m_next = m_header.load();

        while (!m_header.compare_exchange_weak(new_node->m_next, new_node))
            ;
    }

    std::shared_ptr<T> pop()
    {
        node* old_head = m_header.load();

        while (old_head &&
               !m_header.compare_exchange_weak(old_head, old_head->m_next))
            ;
        return old_head ? old_head->m_data : std::shared_ptr<T>();
    }

private:
    std::atomic<int> m_pop_count{0};

public:
    std::shared_ptr<T> pop_safe()
    {
        // 增加引用计数
        m_pop_count++;
        node* old_head = m_header.load();
        while (old_head &&
               !m_header.compare_exchange_weak(old_head, old_head->m_next))
            ;

        std::shared_ptr<T> res;
        if (old_head)
        {
            res.swap(old_head->m_data);
        }
        // 回收数据
        return res;
    }

private:
    std::atomic<node*> to_be_deleted;
    static void delete_nodes(node* nodes)
    {
        while (nodes)
        {
            node* next = nodes->m_next;
            delete nodes;
            nodes = next;
        }
    }

    void try_reclaim(node* old_node)
    {
        if (m_pop_count == 1)
        {
            node* node_to_delete = to_be_deleted.exchange(nullptr);
            if (!--m_pop_count)
            {
                delete_nodes(node_to_delete);
            }
            else if (node_to_delete)
            {
                // xxx
            }
            delete old_node;
        }
        else
        {
            // xxx;
            m_pop_count--;
        }
    }

    void chain_pending_nodes(node* nodes)
    {
        node* last = nodes;
        while (node* next = last->m_next)
        {
            last = next;
        }
    }

    void chain_pending_nodes(node* first, node* last) {}
};

} // namespace gc

// 风险指针
namespace hazard_point {

///<<< 风险指针的上限

/**
 * @brief 简单风险指针
 * @note 线程id 与实际指针
 */
struct hazard_pointer
{
    std::atomic<std::thread::id> id; ///<<< 线程id
    std::atomic<void*> pointer;      ///<<< 指针
};

/// 全局风险指针
constexpr unsigned MAX_HAZARD_POINTERS = 100;
hazard_pointer hazard_pointers[MAX_HAZARD_POINTERS];

/**
 * @brief 风险指针持有器
 */
class hp_owner
{
    hazard_pointer* hp;     //实际风险指针

public:
    hp_owner(const hp_owner&) = delete;
    hp_owner(hp_owner&&) = delete;

    hp_owner() : hp(nullptr)
    {
        for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i)
        {
            std::thread::id old_id; // 默认为0
            if (hazard_pointers[i].id.compare_exchange_strong(
                    old_id, std::this_thread::get_id()))
            {
                hp = &hazard_pointers[i];
                break;
            }
        }
        if (!hp)
        {
            throw std::runtime_error("No hazard pointers available!");
        }
    }

    std::atomic<void*>& get_pointer() { return hp->pointer; }

    ~hp_owner()
    {
        hp->pointer.store(nullptr);
        hp->id.store(std::thread::id());
    }
};

/**
 * @brief 判断当前是否为风险指针
 *
 * @param node
 * @return true
 * @return false
 * @note 该函数内部有全局数据依赖 hazard_pointers
 */
bool outstanding_hazard_pointers_for(void* node)
{
    for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i)
    {
        if (hazard_pointers[i].pointer.load() == node)
        {
            return true;
        }
    }
    return false;
}

template <typename T>
void do_delete(void* p)
{
    delete static_cast<T*>(p);
}

/**
 * @brief 封装需要删除的对象
 */
struct data_to_recliam
{
    void* data;                         ///<<< 需要被删除的数据指针
    std::function<void(void*)> deleter; ///<<< 删除函数
    data_to_recliam* next;              ///<<< 下一个

    template <typename T>
    data_to_recliam(T* p) : data(p), deleter(&do_delete<T>), next(nullptr)
    {
    }

    ~data_to_recliam() { deleter(data); }
};

std::atomic<data_to_recliam*> nodes_to_reclaim;

/**
 * @brief 将 node 加入到待删除的链表中
 *
 * @param node
 */
void add_to_recliam_list(data_to_recliam* node)
{
    node->next = nodes_to_reclaim.load();
    while (!nodes_to_reclaim.compare_exchange_weak(node->next, node))
        ;
}

template <typename T>
void recliam_later(T* t)
{
    add_to_recliam_list(new data_to_recliam(t));
}

/**
 * @brief 删除风险指针
 * @note 该函数内部有数据全局依赖 nodes_to_reclaim
 */
void delete_nodes_with_no_hazards()
{
    data_to_recliam* current = nodes_to_reclaim.exchange(nullptr);
    // 遍历链表
    while (current)
    {
        data_to_recliam* next = current->next;
        if (!outstanding_hazard_pointers_for(current->data))
        {
            delete current;
        }
        else
        {
            add_to_recliam_list(current);
        }
        current = next;
    }
}

/**
 * @brief 基于风险指针的无锁栈
 *
 * @tparam T 栈中存储的数据类型
 */
template <typename T>
class lock_free_stack
{
private:
    struct node
    {
        std::shared_ptr<T> m_data;
        node* m_next;
        node(const T& data) : m_data(std::make_shared<T>(data)), m_next(nullptr)
        {
        }
    };

    std::atomic<node*> m_header;

public:
    void push(const T& data)
    {
        node* new_head = new node(data);
        new_head->m_next = m_header.load();
        while (!m_header.compare_exchange_weak(new_head->m_next, new_head))
            ;
    }

    std::shared_ptr<T> pop()
    {
        std::atomic<void*>& hp = get_hazard_pointer_for_current_thread();
        node* old_header = m_header.load();
        node* tmp;

        // 将风险指针设为head指针
        do
        {
            tmp = old_header;
            hp.store(old_header);
            old_header = m_header.load();
        } while (old_header != tmp);

        while (old_header && !m_header.compare_exchange_strong()(
                                 old_header, old_header->m_next))
            ;
        // 清除风险指针
        hp.store(nullptr);

        std::shared_ptr<T> res;
        if (old_header)
        {
            res.swap(old_header->m_data);

            // 删除前检查风险指针
            if (outstanding_hazard_pointers_for(old_header))
            {
                // 有其他线程在使用，需要放回链表中，一会删除
                reclaim_later(old_header);
            }
            else
            {
                // 没有人使用了直接删除
                delete old_header;
            }
            //
            delete_nodes_with_no_hazards();
        }
        return res;
    }

    void reclain_later(node* node);

    /**
     * @brief node 不在风险指针之中
     *
     * @param node
     * @return true
     * @return false
     */
    bool outstanding_hazard_pointers_for(node* node)
    {
        for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i)
        {
            if (hazard_pointers[i].pointer.load() == node)
            {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief 获取当前线程对象的风险指针
     *
     * @return std::atomic<void*>&
     */
    std::atomic<void*>& get_hazard_pointer_for_current_thread()
    {
        thread_local static hp_owner hazard;
        return hazard.get_pointer();
    }
    void delete_nodes_with_no_hazards();
};
} // namespace hazard_point

int main() { return 0; }