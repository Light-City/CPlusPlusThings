> 来自 [C++那些事网站](https://light-city.github.io/stories_things/stl-source-analysis/stl-iterator-invalidation/) 的文章，由站点同步脚本导出。

# STL 迭代器失效

> https://stackoverflow.com/questions/6438086/iterator-invalidation-rules-for-c-containers

插入
顺序容器

- vector：在插入点之前的所有迭代器和引用都不受影响，除非新容器的大小超过之前的容量（在这种情况下，所有迭代器和引用都将失效）[23.3.6.5/1]
- deque：所有迭代器和引用都将失效，除非插入的元素位于deque的两端（前端或后端）（在这种情况下，所有迭代器将失效，但对元素的引用不受影响）[23.3.3.4/1]
- list：所有迭代器和引用不受影响 [23.3.5.4/1]
- forward_list：所有迭代器和引用不受影响（适用于insert_after）[23.3.4.5/1]
- array：（不适用）

关联容器

- [multi]{set,map}：所有迭代器和引用不受影响 [23.2.4/9]

无序关联容器

- unordered_[multi]{set,map}：当发生重哈希时，所有迭代器将失效，但引用不受影响 [23.2.5/8]。如果插入不会导致容器的大小超过z * B，则不会发生重哈希，其中z是最大负载因子，B是当前的桶数 [23.2.5/14]

容器适配器

- stack：继承自底层容器
- queue：继承自底层容器
- priority_queue：继承自底层容器

删除
顺序容器

- vector：删除点及其后的每个迭代器和引用都将失效 [23.3.6.5/3]
- deque：删除最后一个元素仅使迭代器和引用到被删除的元素和past-the-end迭代器失效；删除第一个元素仅使迭代器和引用到被删除的元素失效；删除任何其他元素将使所有迭代器和引用（包括past-the-end迭代器）失效 [23.3.3.4/4]
- list：仅对被删除元素的迭代器和引用失效 [23.3.5.4/3]
- forward_list：仅对被删除元素的迭代器和引用失效（适用于erase_after）[23.3.4.5/1]
- array：（不适用）

关联容器

- [multi]{set,map}：仅对被删除元素的迭代器和引用失效 [23.2.4/9]

无序关联容器

- unordered_[multi]{set,map}：仅对被删除元素的迭代器和引用失效 [23.2.5/13]

容器适配器

- stack：继承自底层容器
- queue：继承自底层容器
- priority_queue：继承自底层容器

调整大小
- vector：与插入/删除操作相同 [23.3.6.5/12]
- deque：与插入/删除操作相同 [23.3.3.3/3]
- list：与插入/删除操作相同 [23.3.5.3/1]
- forward_list：与插入/删除操作相同 [23.3.4.5/25]
- array：（不适用）

注释1
除非另有明确规定（无论是显式规定还是通过其他函数定义函数），调用容器成员函数或将容器作为参数传递给库函数不应使该容器内对象的迭代器失效或改变其值。 [23.2.1/11]

注释2
没有任何swap()函数会使指向被交换容器元素的引用、指针或迭代器失效。[注：end()迭代器不指向任何元素，因此它可能会失效。— 结束注释] [23.2.1/10]

注释3
除上述有关swap()的警告外，不清楚“end”迭代器是否受上述每个容器规则的约束；无论如何，你应该假设它们是受约束的。

注释4
vector和所有无序关联容器支持reserve(n)，它保证至少在容器大小增加到n之前不会发生自动调整大小。对于无序关联容器应谨慎，因为未来的提案将允许指定最小负载因子，这将允许在足够的删除操作减少容器大小低于最小值后，在插入时发生重哈希；因此在删除后，该保证应被视为可能无效。

> https://en.cppreference.com/w/cpp/container#Iterator_invalidation

```cpp
for (auto it = c.begin(); it != c.end(); /*inc in loop*/)
{
   if (it->shouldErase())
   {
         it = c.erase(it);
         continue;
    }
    ...
    ++it;
}
```



```cpp
auto is_dead = [](auto const& e) { return !(e->is_alive()); };

std::erase_if(m_entities, is_dead);
std::erase_if(m_entity_map, is_dead);
```

erase_if or remove_if

> https://stackoverflow.com/questions/39019806/using-erase-remove-if-idiom



> https://kera.name/articles/2011/06/iterator-invalidation-rules-c0x/