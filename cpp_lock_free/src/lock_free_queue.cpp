#include <memory>
#include <atomic>

template <typename T>
class lock_free_queu
{
private:
    struct node
    {
        std::shared_ptr<T> m_data;
        node* m_next;
        node() : m_next(nullptr) {}
    };

    std::atomic<node*> m_header;
    std::atomic<node*> m_tail;

    node* pop_head()
    {
        const node* old_head = m_header.load();

        if (old_head == m_tail.load())
        {
            return nullptr;
        }
        m_header.store(old_head->m_next);
        return old_head;
    }

    node* pop_head_safe() { node* old_head = m_header.load();

        
    
     }

public:
    lock_free_queu() : m_header(new node), m_tail(m_header.load) {}

    lock_free_queu(const lock_free_queu&) = delete;
    lock_free_queu(lock_free_queu&&) = delete;

    ~lock_free_queu()
    {
        while (const node* old_head = m_header.load())
        {
            m_header.store(old_head->m_next);
            delete old_head;
        }
    }

    std::shared_ptr<T> pop()
    {
        node* old_head = m_header.load();
        if (!old_head)
        {
            return std::shared_ptr<T>();
        }
        const std::shared_ptr<T> res(old_head->m_data);
        delete old_head;
        return res;
    }

    void push(T data) {}
};

int main() { return 0; }
