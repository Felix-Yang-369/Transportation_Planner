#include <gtest/gtest.h>
#include "DijkstraPathRouter.h"

TEST(DijkstraPathRouter, SimpleTest){
    CDijkstraPathRouter PathRouter;

    EXPECT_EQ(PathRouter.VertexCount(),0);
    auto VertexID = PathRouter.AddVertex(std::string("foo"));
    EXPECT_EQ(PathRouter.VertexCount(),1);
    auto VetextTag = std::any_cast<std::string>(PathRouter.GetVertexTag(VertexID));
    EXPECT_EQ(VetextTag,"foo");
    
}

TEST(DijkstraPathRouter, ShortestPath){
    CDijkstraPathRouter PathRouter;
    /*
                5   
       A ---> B --->C
          4  2|  ___^
              V / 1
              D
    
    */
    auto VertexA = PathRouter.AddVertex(std::string("A"));
    auto VertexB = PathRouter.AddVertex(std::string("B"));
    auto VertexC = PathRouter.AddVertex(std::string("C"));
    auto VertexD = PathRouter.AddVertex(std::string("D"));
    EXPECT_EQ(PathRouter.VertexCount(),4);
    EXPECT_TRUE(PathRouter.AddEdge(VertexA,VertexB,4.0));
    EXPECT_TRUE(PathRouter.AddEdge(VertexB,VertexC,5.0));
    EXPECT_TRUE(PathRouter.AddEdge(VertexB,VertexD,2.0));
    EXPECT_TRUE(PathRouter.AddEdge(VertexD,VertexC,1.0));
    std::vector<CPathRouter::TVertexID> Path;
    EXPECT_EQ(PathRouter.FindShortestPath(VertexA,VertexC,Path), 7.0);
    std::vector<CPathRouter::TVertexID> ExpectedPath{VertexA,VertexB,VertexD,VertexC};
    EXPECT_EQ(Path,ExpectedPath);
    
}