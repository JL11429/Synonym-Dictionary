/**
* \file SynonymDictionary.h
*/

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>

#ifndef _SYNONYMDICTIONARY__H
#define _SYNONYMDICTIONARY__H

namespace synonymDictionaryUtils
{

   
   class SynonymDictionary
   {
   public:

      
      SynonymDictionary();

      
      SynonymDictionary(std::ifstream &file);


      ~SynonymDictionary();

     
      void addRootWord(const std::string& rootWord);

      
      void addInflection(const std::string& rootWord, const std::string& inflexedWord);

      
      void addSynonym(const std::string& rootWord, const std::string& wordSynonym, int& groupNumber);

      
      void removeRootWord(const std::string& rootWord);
     
      
      void removeInflection(const std::string& rootWord, const std::string& inflexedWord);
   
      
      void removeSynonym(const std::string& rootWord, const std::string& wordSynonym, int& groupNumber);

      
      bool isEmpty() const;

      
      int numberOfRadicals() const;

      friend std::ostream& operator<<(std::ostream& out, const SynonymDictionary& d)
      {
        
    	  if (d.root != 0)
            
    	  {
              
    		  std::queue<NodeSynonymDictionary*> file;
    		  std::queue<std::string> fileLevel;

    		  NodeSynonymDictionary * nodeDicotemp;
    		  std::string levelTemp;

    		  int hg = 0, hd = 0;

    		  file.push(d.root);
    		  fileLevel.push("1");
    		  while (!file.empty())
                    
    		  {
    			  nodeDicotemp = file.front();
    			  levelTemp = fileLevel.front();
    			  out << nodeDicotemp->rootWord;
    			  if (nodeDicotemp->left == 0) hg = -1; else hg = nodeDicotemp->left->height;
    			  if (nodeDicotemp->right == 0) hd = -1; else hd = nodeDicotemp->right->height;
    			  out << ", " << hg - hd;
    			  out << ", " << levelTemp;
    			  out << std::endl;
    			  file.pop();
    			  fileLevel.pop();
                          
    			  if (nodeDicotemp->left != 0)
    			  {
    				  file.push(nodeDicotemp->left);
    				  fileLevel.push(levelTemp + ".1");
    			  }
    			  if (nodeDicotemp->right != 0)
    			  {
    				  file.push(nodeDicotemp->right);
    				  fileLevel.push(levelTemp + ".2");
    			  }
    		  }
    	  }
          
    	  return out;
      }


      
      void loadSynonymDictionary(std::ifstream & p_file);

      
      std::string findRootWord(const std::string& word) const;

      
      float similarity(const std::string& word1, const std::string& word2) const;

      
      int getNumberOfMeanings(std::string rootWord) const;

      
      std::string getMeaning(std::string rootWord, int position) const;

      
      std::vector<std::string> getSynonyms(std::string rootWord, int position) const;

      
      std::vector<std::string> getInflections(std::string rootWord) const;
	  
      
      bool isAVLTree() const;
                   
      

   private:

      class NodeSynonymDictionary
      {
      public:
         std::string rootWord;             		
         std::list<std::string> inflections;   	
         std::vector<int> rootWordBelongsToSynonymGroups;   		
         int height;                    		
         NodeSynonymDictionary *left, *right;	
         NodeSynonymDictionary(const std::string& m) : rootWord(m), height(0), left(0), right(0) { }
      };

      NodeSynonymDictionary *root;                           
      int numberOfRootWords;                                      
      std::vector< std::list<NodeSynonymDictionary*> > SynonymGroups;   
      	  	  	  	  	  	  	  	  	
        
      
      void _insertRootWordAVL(NodeSynonymDictionary*& node, const std::string& rootWord);
            
     
      void _balanceNode(NodeSynonymDictionary*& node);
      
      
      void _updateNodeHeight(NodeSynonymDictionary*& node);
      
      
      int _height(NodeSynonymDictionary* node) const;
      

      int _amplitudeOfImbalance(NodeSynonymDictionary * p_tree) const;

      
      bool _isImbalancedOnLeftSide(NodeSynonymDictionary * & p_tree) const;
      
      
      bool _isImbalancedOnRightSide(NodeSynonymDictionary * & p_tree) const;
      
      
      bool _subtreeLeansLeft(NodeSynonymDictionary * & p_tree) const;
      

      
      bool __subtreeLeansRight(NodeSynonymDictionary * & p_tree) const;
      
      
      void _leftLeftRotation(NodeSynonymDictionary * & p_CriticalNode);
      

      void _rightRightRotation(NodeSynonymDictionary * & p_CriticalNode);
      
      
      void _leftRightRotation(NodeSynonymDictionary * & p_CriticalNode);

      
      void _rightLeftRotation(NodeSynonymDictionary * & p_CriticalNode);

      
      NodeSynonymDictionary* _findRootWord(NodeSynonymDictionary* node, const std::string& word) const;
      

      void _traverseTree(NodeSynonymDictionary* node, std::vector<NodeSynonymDictionary*>& nodes) const;

      
      std::vector<NodeSynonymDictionary*> traverseTree() const ;

      
      void _removeSubtree(NodeSynonymDictionary*& node);
      

      void _removeRootWordFromAVL(NodeSynonymDictionary*& node, const std::string& rootWord);
      
      
      void _removeRightMinSuccessorNode(NodeSynonymDictionary* node);
              
   };

}

#endif
