#include <iostream>
#include <unordered_map>
#include <vector>

//-----------------------------------------------------------------------------

void task1()
{
    int num = 4494;
    char binary[sizeof(int) * 8 + 1];
    int it = 0;
    for (unsigned i = (1 <<  - 1); i > 0; i = i / 2)
        binary[it++] = (num & i) ? '1' : '0';
    binary[it] = '\0';
    printf("task1 = %s\n", binary);
}

//-----------------------------------------------------------------------------

void RemoveDups(char* str, int size)
{
    int j = 0;
    for(int i = 0; i < size; ++i)
    {
        if(str[i] == str[i+1])
            continue;
        str[++j] = str[i+1];
    }
}   

void task2()
{
    const int SIZE = 14;
    char data[SIZE] = "AAA BBB   AAA"; // have no \0 then i use size
    RemoveDups(data, SIZE);
    printf("task2 = %s\n", data); // "A B A"
}

//-----------------------------------------------------------------------------

struct ListNode 
{
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode* rand = nullptr;
    std::string data; 
};

class List 
{
public:
    void Serialize(FILE* file)
    {
        if(!head) return;

        ListNode* iter = head;
        do
        {
            uintptr_t uptr = reinterpret_cast<uintptr_t>(iter);

            fwrite(&uptr, sizeof(uintptr_t), 1, file);
            uptr = reinterpret_cast<uintptr_t>(iter->rand);

            fwrite(&uptr, sizeof(uintptr_t), 1, file);
            size_t data_sz = iter->data.size();

            fwrite(&data_sz, sizeof(size_t), 1, file);
            fwrite(iter->data.c_str(), sizeof(char), data_sz, file);

        } while((iter = iter->next));
    }

    void Deserialize(FILE* file)
    {
        using node_ptr = uintptr_t;
        using rand_ptr = uintptr_t;
        std::unordered_map<node_ptr, ListNode*> nodes;
        std::unordered_map<node_ptr, rand_ptr> nodes_to_rand_key;
        nodes.insert({reinterpret_cast<uintptr_t>(nullptr), nullptr});

        uintptr_t default_key, random_key;
        while(fread(&default_key, sizeof(uintptr_t), 1, file) == 1 &&
              fread(&random_key, sizeof(uintptr_t), 1, file) == 1)
        {
            size_t read_sz = 0;
            fread(&read_sz, sizeof(size_t), 1, file);
            char temp[read_sz];
            fread(&temp, sizeof(char), read_sz, file);
            Push(temp);
            nodes.insert({default_key, (tail) ? tail : head});
            if(random_key != 0)
                nodes_to_rand_key.insert({default_key, random_key});
        }

        for(const auto& it : nodes_to_rand_key)
            nodes.at(it.first)->rand = nodes.at(it.second); //assumed that we set only valid pointers to existing nodes
    }

    void Push(const std::string& data)
    {
        auto new_node = new ListNode();
        new_node->data = data;

        if(!head)
        {
            head = new_node;
            return;
        }
        
        if(!tail)
        {
            tail = new_node;
            tail->prev = head;
            head->next = tail;
            return;
        }

        tail->next = new_node;
        new_node->prev = tail;
        tail = tail->next;

        count++;
    }

    bool Compare(List* other)
    {
        if(count != other->count)
            return false;
    
        ListNode* other_it = other->head;
        ListNode* this_it = head;
        while(other_it && this_it)
        {
            if(other_it->data != this_it->data)
                return false;
            other_it = other_it->next;
            this_it = this_it->next;
        }
        return true;
    }

    void ShortDescr()
    {
        std::cout << "--------List--------" << std::endl;
        ListNode* it = head;
        while(it)
        {
            std::cout 
            << "current - " << it << std::endl
            << "   data - " << it->data << std::endl
            << "   next - " << it->next << std::endl
            << "   prev - " << it->prev << std::endl
            << "   rand - " << it->rand << std::endl;
            it = it->next;
        }
        std::cout << "--------------------" << std::endl;
    }

    ListNode* get_head() { return head; }

private:
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    int count = 0;
};

int main()
{ 
    // simple tests

    task1();
    task2();

    //---------------

    FILE* file = fopen("./data.bin", "wb");
    List lst;
    lst.Push("111111");
    lst.Push("2222");
    lst.Push("3333");
    lst.get_head()->rand = lst.get_head()->next->next;
    lst.Push("4444");
    lst.Push("5555");
    lst.get_head()->next->next->rand = lst.get_head();
    lst.ShortDescr();
    lst.Serialize(file);
    fclose(file);

    FILE* file2 = fopen("./data.bin", "rb");
    List lst2;
    lst2.Deserialize(file2);
    lst2.ShortDescr();
    if(lst.Compare(&lst2))
        std::cout << "Lists same!" << std::endl;
    
    fclose(file2);
    return 0;
}