Rationale of Tests
This will be a rationale of the way I went about testing and why I went about testing this way.
I will be testing my functions in an order which I think makes the most sense.

1. Constructors (Part 1/2):
    a) graph();
    b) graph(std::initializer_list<N> il);
    c) graph(InputIt first, InputIt last);

2. Modifiers (Part 1/2):
    a) auto insert_node(N const& value) -> bool;
    b) auto insert_edge(N const& src, N const& dst, E const& weight) -> bool;

3. Comparisons (Part 1/1):
    a) [[nodiscard]] auto operator==(graph const& other) -> bool;

4.. Iterator (Part 1/1):
    a. iterator();
    b. explicit iterator(unspecified);
    c. auto operator*() -> reference;
    d. auto operator++() -> iterator&;
    e. auto operator++(int) -> iterator;
    f. auto operator--() -> iterator&;
    g. auto operator--(int) -> iterator;

    Iterator Comparison:
    a. auto operator==(iterator const& other) -> bool;

5. Constructors (Part 2/2):
    d) graph(graph&& other) noexcept;
    e) auto operator=(graph&& other) noexcept -> graph&;
    f) graph(graph const& other);
    g) auto operator=(graph const& other) -> graph&;

6. Modifiers (Part 2/2):
    c) auto replace_node(N const& old_data, N const& new_data) -> bool;
    d) auto merge_replace_node(N const& old_data, N const& new_data) -> void; ( c and d done together)

    e) auto erase_node(N const& value) -> bool;
    f) auto erase_edge(iterator i) -> iterator;
    g) auto erase_edge(iterator i, iterator s) -> iterator;
    h) auto erase_edge(N const& src, N const& dst, E const& weight) -> bool;
    i) auto clear() noexcept -> void;

7. Extractor (Part 1/1):
    a. friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream&;

8. Testing for CONSTNESS SEPERATELY!


Detailed Rationale :
Firstly, before anything else I want to address circular testing as it is one of the main reasons
why tests for all the accessor functions have not been done. I use these functions to validate
the functionality of the other functions. Despite, this I build the validity and reliability of
my program my carefully planning the progression of my testing. I also want to emphasise that I try
exemplify the linear progression of my testing by using the previously tested functions to
further validate my current functions.

I will give you guys a quick run down of my testing.
Firstly, I test my constructors as without the constructors working nothing else can be properly
tested and/or properly implemented. However, I don't test all of my constructors all at once!
That's because in order to test it to the extent that the total reliability of constructors such as
the copy constructor or move constructor, I would need to use some modifiers. Hence, I take the most
intuitive route and that is to test the modifiers specifically the insert_node and insert_edge.
Thus, allowing me to more effeciently test other important constructors and assignments
such as the copy and moves. But, instead of jumping to this route, I instead test the functionality
of my comparison function, as I have everything that I need to test it and I can utilise this function
for the effecient testing other the next few functions. Then, I realised to test the functionality of
the modifier functions later on and the constructors too, testing Iterators would be a better plan
of aciton. Which is what I did, and this simply helped me to speed up my testing too.
However, I could only "test" a chunk of iterator functions, and not test stuff like begin(), end()
find(), due to reasons such as circular testing and due to reasons such as diminishing returns
if I really wanted to find the reliability of these functions. I would have to use the other
accessors to rigorously test these functions, but it doesnt make sense to since an error caused by
these functions can be a) easily picked up and the complexity of these functions were relatively easy
too, so hence due to diminishing returns I decided not to test these function.

Then I just, moved on and tested my constructors.

I used the copy and move constructors a lot to validy the rest of my modifier functions, since it
is just a more reasonable and easier thing to do, then generating copies yourself. The comparison
== for graph was extremeley handy as well.

An important aspect I'd like to highlight about testing the modifiers is, how they are all
put it into 1 Test case area, and the actual tests for some of them divided by into Sections.
The reason I did this was so I could test for multiple scenarios and cases by just generating
a complex and large graph. Yes, this probably isn't a good way to test your functions during
development but it is a good way to make a strong statement about how robust and reliable the
modifier functions are. The Copy Constructor and Comparison constructor made it easy to compare
before and after of graphs once a function had been applied. Thus, testing in my modifiers was
made a lot smooth due to the testing that occured before.

Following this, we tested ostream function.

Lastly, I wanted to test the constness of my functions within a specific area as opposed to spreading
the const testing throughout, and the reason I did this is because firstly, we know modifiers
have to be non const. Secondly, only accessors and some of the constructors can you really
do anything with a const graph. I figured doing it this way, I'd probably not be as forgetful
in terms of all the different testing I need to do for them as well. 