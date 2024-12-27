/**
* \file SynonymDictionary.cpp
*/

#include "SynonymDictionary.h"

namespace synonymDictionaryUtils
{

  
    SynonymDictionary::SynonymDictionary() : root(nullptr), numberOfRootWords(0){}

    
    SynonymDictionary::SynonymDictionary(std::ifstream &file): root(nullptr), numberOfRootWords(0) {}


    void SynonymDictionary::loadSynonymDictionary(std::ifstream& file)
    {
    if (!file.is_open())
        throw std::logic_error("SynonymDictionary::loadSynonymDictionary: the file is not open !");

            std::string line;
            std::string buffer;
            std::string rootWord;
            int category = 1;

            while (!file.eof()) 
            {
        std::getline(file, line);

                    if (line == "$")
                    {
                            category = 3;
                            std::getline(file, line);
                    }
                    if (category == 1)
                    {
                            rootWord = line;
                            addRootWord(rootWord);
                            category = 2;
                    }
                    else if (category == 2)
                    {
                            std::stringstream ss(line);
                            while (ss >> buffer)
                                    addInflection(rootWord, buffer);
                            category = 1;
                    }
                    else
                    {
                            std::stringstream ss(line);
                            ss >> rootWord;
                            ss >> buffer;
                            int position = -1;
                            addSynonym(rootWord, buffer, position);
                            while (ss >> buffer)
                                    addSynonym(rootWord, buffer, position);
                    }
            }
    }
    

    
    SynonymDictionary::~SynonymDictionary() 
    {
        _removeSubtree(root);
    }
    
    
    void SynonymDictionary::addRootWord(const std::string& rootWord)
    {
        
        if (_findRootWord(root,rootWord) != nullptr) 
          {
            throw std::logic_error("Root word already exist");
          }
               
        _insertRootWordAVL(root, rootWord);
      
        if (_findRootWord(root,rootWord) == nullptr) 
          {
            throw std::logic_error("Insertion did not work");
          }
    }

    
    void SynonymDictionary::addInflection(const std::string& rootWord, const std::string& inflexedWord)
    {
        NodeSynonymDictionary* node = _findRootWord(root, rootWord);
        
        if (!node)
            throw std::logic_error("Root word does not exist");
        
        if (std::find(node->inflections.begin(), node->inflections.end(), inflexedWord) != node->inflections.end())
            throw std::logic_error("Inflection already present");
        
        node->inflections.push_back(inflexedWord);
    }

    
    void SynonymDictionary::addSynonym(const std::string& rootWord, const std::string& wordSynonym, int& groupNumber) 
    {
        NodeSynonymDictionary* node = _findRootWord(root, rootWord);
        
        
        if (!node) 
          {
            throw std::logic_error("Root word does not exist");
          }
        
        for (const auto& groupe : node->rootWordBelongsToSynonymGroups) {
            for (const auto& syn : SynonymGroups[groupe]) {
                if (syn->rootWord == wordSynonym) 
                  {
                    throw std::logic_error("The synonym is already within the group of synonyms");
                  }
            }
        }
        
        if (groupNumber == -1) 
          {
            groupNumber = SynonymGroups.size();
            SynonymGroups.push_back(std::list<NodeSynonymDictionary*>());
          } 
        else if (groupNumber < 0 || groupNumber >= SynonymGroups.size()) 
          {
            throw std::logic_error("Incorrect group number");
          }

         
        if (std::find(node->rootWordBelongsToSynonymGroups.begin(), node->rootWordBelongsToSynonymGroups.end(), groupNumber) == node->rootWordBelongsToSynonymGroups.end()) 
          {
            node->rootWordBelongsToSynonymGroups.push_back(groupNumber);
          }
        
          if (_findRootWord(root,wordSynonym) == 0) 
            {
              _insertRootWordAVL(root, wordSynonym);
            }
        
        SynonymGroups[groupNumber].push_back(new NodeSynonymDictionary(wordSynonym));
    }


    void SynonymDictionary::removeRootWord(const std::string& rootWord)
    {
        if (root == nullptr) 
          {
            throw std::logic_error("Tree is emtpy");
          }
        
         
        NodeSynonymDictionary* node = _findRootWord(root, rootWord);
        
        if (node == nullptr) 
          {
            throw std::logic_error("Root word does not exist");
          }
                
        _removeRootWordFromAVL(root, rootWord);
    }

    
    void SynonymDictionary::removeInflection(const std::string& rootWord, const std::string& inflexedWord)
    {
        if (root == nullptr) 
          {
            throw std::logic_error("Tree is empty");
          }

        NodeSynonymDictionary* node = _findRootWord(root, rootWord);
        
        if (node == nullptr) 
          {
            throw std::logic_error("Root word does not exist");
          }

        auto it = std::find(node->inflections.begin(), node->inflections.end(), inflexedWord);
        if (it == node->inflections.end()) 
          {
            throw std::logic_error("Inflection does not exist");
          }
       
        node->inflections.erase(it);
    }

    
    void SynonymDictionary::removeSynonym(const std::string& rootWord, const std::string& wordSynonym, int& groupNumber)
    {
       
        NodeSynonymDictionary* node = _findRootWord(root, rootWord);
        
        if (!node) 
          {
            throw std::logic_error("Root word does not exist");
          }

        
        if (groupNumber < 0 || groupNumber >= SynonymGroups.size()) 
          {
            throw std::logic_error("Incorrect group number");
          }

         
        auto& groupe = SynonymGroups[groupNumber];
        auto it = groupe.end();
        
        
        for (auto iter = groupe.begin(); iter != groupe.end(); ++iter) {
            if ((*iter)->rootWord == wordSynonym) 
              {
                it = iter;
                break;
              }
        }
        
        if (it == groupe.end()) 
          {
            throw std::logic_error("Synonym does not exist in mentioned group");
          }
        
        delete *it;
                
        groupe.erase(it);

        if (groupe.empty()) 
          {
            SynonymGroups.erase(SynonymGroups.begin() + groupNumber);
            
            for (NodeSynonymDictionary* node : traverseTree()) {
                auto& rootWordBelongsToSynonymGroups = node->rootWordBelongsToSynonymGroups;

                auto it = std::remove(rootWordBelongsToSynonymGroups.begin(), rootWordBelongsToSynonymGroups.end(), groupNumber);
                rootWordBelongsToSynonymGroups.erase(it, rootWordBelongsToSynonymGroups.end());

                for (int& groupeNum : rootWordBelongsToSynonymGroups) {
                    if (groupeNum > groupNumber) 
                      {
                        groupeNum--;
                      }
                }
            }
          }
    }


    bool SynonymDictionary::isEmpty() const
    {
        return numberOfRootWords == 0;
    }

    
    int SynonymDictionary::numberOfRadicals() const
    {
        return numberOfRootWords;
    }
    

    std::string SynonymDictionary::findRootWord(const std::string& word) const
    {
        if (root == nullptr) 
          {
            throw std::logic_error("Tree is empty");
          }

        NodeSynonymDictionary* bestNode = nullptr;
        
        float bestSimilarity = 0.0;
                 

        std::vector<NodeSynonymDictionary*> nodes = traverseTree();

        for (NodeSynonymDictionary* node : nodes) {
                       
            for (const auto& inflection : node->inflections) {
                
                if (inflection == word) 
                  
                  {

                    float sim = similarity(node->rootWord, word);
                    
                    if (sim > bestSimilarity) 
                      {
                        bestSimilarity = sim;
                        bestNode = node;
                      }
                  }
            }
        }
         
        if (bestNode == nullptr) 
          {
            throw std::logic_error("Inflection is not in list of inflections of root word");
          }

        return bestNode->rootWord;
    }

    float SynonymDictionary::similarity(const std::string& word1, const std::string& word2) const 
    {
        size_t lenght1 = word1.size();
        size_t lenght2 = word2.size();
        std::vector<std::vector<float>> dp(lenght1 + 1, std::vector<float>(lenght2 + 1));
       
        float insertionCost = 1.0f;     
        float removeCost = 0.5f;     
        float SubstitutionCost = 1.5f; 
         
        for (size_t i = 0; i <= lenght1; ++i) {
            for (size_t j = 0; j <= lenght2; ++j) {
                if (i == 0)
                    dp[i][j] = j * insertionCost;
                else if (j == 0)
                    dp[i][j] = i * removeCost;
                else if (word1[i - 1] == word2[j - 1])
                    dp[i][j] = dp[i - 1][j - 1];
                else
                    dp[i][j] = std::min({
                        dp[i - 1][j] + removeCost,
                        dp[i][j - 1] + insertionCost,
                        dp[i - 1][j - 1] + SubstitutionCost
                    });
            }
        }

        float maxLenght = static_cast<float>(lenght1 + lenght2);
        return 1.0f - dp[lenght1][lenght2] / maxLenght;
    }
    
    
    int SynonymDictionary::getNumberOfMeanings(std::string rootWord) const
    {

        NodeSynonymDictionary* node = _findRootWord(root, rootWord);
        
        if (!node) 
          {
            throw std::logic_error("Root word does not exist");
          }

        return node->rootWordBelongsToSynonymGroups.size();
    }


    std::string SynonymDictionary::getMeaning(std::string rootWord, int position) const
    {
 
        NodeSynonymDictionary* node = _findRootWord(root, rootWord);
        
        if (!node) 
          {
            throw std::logic_error("Root word does not exist");
          }

        if (position < 0 || position >= node->rootWordBelongsToSynonymGroups.size()) 
          {
            throw std::logic_error("Position does not exist");
          }
        
        int GroupNumber = node->rootWordBelongsToSynonymGroups[position];

        if (GroupNumber < 0 || GroupNumber >= SynonymGroups.size()) 
          {
            throw std::logic_error("Incorrect group number");
          }

        auto& groupe = SynonymGroups[GroupNumber];
        
        if (groupe.empty()) 
          {
            throw std::logic_error("Synonym group is empty");
          }

        return groupe.front()->rootWord;
    }

    
    std::vector<std::string> SynonymDictionary::getSynonyms(std::string rootWord, int position) const
    {

        NodeSynonymDictionary* node = _findRootWord(root, rootWord);
        
        if (!node) 
          {
            throw std::logic_error("Root word does not exist");
          }
        
        if (position < 0 || position >= node->rootWordBelongsToSynonymGroups.size()) 
          {
            throw std::logic_error("Position does not exist");
          }

        int GroupNumber = node->rootWordBelongsToSynonymGroups[position];
        
        if (GroupNumber < 0 || GroupNumber >= SynonymGroups.size()) 
          {
            throw std::logic_error("Incorrect group number");
          }

        auto& groupe = SynonymGroups[GroupNumber];
        std::vector<std::string> synonyms;
        
        for (const auto& syn : groupe) {
            synonyms.push_back(syn->rootWord);
        }

        return synonyms;
    }

    
    std::vector<std::string> SynonymDictionary::getInflections(std::string rootWord) const
    {
        NodeSynonymDictionary* node = _findRootWord(root, rootWord);
        
        if (!node) 
          {
            throw std::logic_error("Root word does not exist");
          }

        std::vector<std::string> inflections;

        for (const auto& inflection : node->inflections) {
            inflections.push_back(inflection);
        }

        return inflections;
    }

    bool SynonymDictionary::isAVLTree() const 
    {
	if (isEmpty()) 
          {
		return true;
	  }
	std::queue<NodeSynonymDictionary*> Q;
        
	NodeSynonymDictionary * temporary;

	Q.push(root);
	while (!Q.empty()) {
		temporary = Q.front();
               
		if (_amplitudeOfImbalance(temporary) > 1) 
                  {
			return false;
		  }
		Q.pop();
                
		if (temporary->left != 0)
			Q.push(temporary->left);
                
		if (temporary->right != 0)
			Q.push(temporary->right);
	}
        
	return true;
    }

    void SynonymDictionary::_insertRootWordAVL(NodeSynonymDictionary*& node, const std::string& rootWord) 
    {
        if (node == nullptr) 
          {
            node = new NodeSynonymDictionary(rootWord);
            numberOfRootWords++;
            return;
          } 

        else if (rootWord < node->rootWord) 
          {
            _insertRootWordAVL(node->left, rootWord);
          } 

        else 
          {
            _insertRootWordAVL(node->right, rootWord);
          }

        _updateNodeHeight(node);

        _balanceNode(node);
    }    

    void SynonymDictionary::_balanceNode(NodeSynonymDictionary*& node) 
    {

        if (node == nullptr) 
          {
            return;
          }

        if (_isImbalancedOnLeftSide(node)) 
          {

            if (__subtreeLeansRight(node->left)) 
              {
                _leftRightRotation(node);
              } 

            else 
              {
                _leftLeftRotation(node);
              }
          } 

        else if (_isImbalancedOnRightSide(node)) 
          {

            if (_subtreeLeansLeft(node->right)) 
              {
                _rightLeftRotation(node);
              } 
 
            else 
              {
                _rightRightRotation(node);
              }
          }

        if (std::abs(_height(node->left) - _height(node->right)) >= 2) 
          {
            throw std::logic_error("Amplitude of imbalance is greater or equal to 2");
          }
    }

    
    void SynonymDictionary::_updateNodeHeight(NodeSynonymDictionary*& node) 
    {
        if (node != nullptr) 
          {
            node->height = 1 + std::max(_height(node->left), _height(node->right));
          }
    }

    
    int SynonymDictionary::_height(NodeSynonymDictionary* node) const 
    {
        return node ? node->height : -1;
    }

    
    int SynonymDictionary::_amplitudeOfImbalance(NodeSynonymDictionary * p_tree) const 
    {
        return std::abs(_height(p_tree->left) - _height(p_tree->right));
    }
    
    bool SynonymDictionary::_isImbalancedOnLeftSide(NodeSynonymDictionary *& p_tree) const 
    {
        if (p_tree == 0) 
          {
            return false;
          }
        
        return _height(p_tree->left) - _height(p_tree->right) >= 2;
    }

    bool SynonymDictionary::_isImbalancedOnRightSide(NodeSynonymDictionary * & p_tree) const 
    {
        if (p_tree == 0) 
          {
                return false;
          }
        
        return _height(p_tree->right) - _height(p_tree->left) >= 2;
    }


    bool SynonymDictionary::_subtreeLeansLeft(NodeSynonymDictionary *& p_tree) const 
    {
        if (p_tree == 0) 
          {
                return false;
          }
        
        return (_height(p_tree->left) > _height(p_tree->right));
    }

    bool SynonymDictionary::__subtreeLeansRight(NodeSynonymDictionary *& p_tree) const 
    {
        if (p_tree == 0) 
          {
            return false;
          }
        
        return (_height(p_tree->right) > _height(p_tree->left));
   }
    

   void SynonymDictionary::_leftLeftRotation(NodeSynonymDictionary * & p_CriticalNode) 
   {
        NodeSynonymDictionary * SubCriticalNode = p_CriticalNode->left;
        
        p_CriticalNode->left = SubCriticalNode->right;
        
        SubCriticalNode->right = p_CriticalNode;

        _updateNodeHeight(p_CriticalNode);
        
        _updateNodeHeight(SubCriticalNode);

        p_CriticalNode = SubCriticalNode;
   }
   

   void SynonymDictionary::_rightRightRotation(NodeSynonymDictionary *& p_CriticalNode) 
   {
        NodeSynonymDictionary * SubCriticalNode = p_CriticalNode->right;
        
        p_CriticalNode->right = SubCriticalNode->left;
        
        SubCriticalNode->left = p_CriticalNode;
        
        _updateNodeHeight(p_CriticalNode);
        
        _updateNodeHeight(SubCriticalNode);

        p_CriticalNode = SubCriticalNode;
   }
   
   void SynonymDictionary::_leftRightRotation(NodeSynonymDictionary * & p_CriticalNode) 
   {
        _rightRightRotation(p_CriticalNode->left);
        
        _leftLeftRotation(p_CriticalNode);
   }


   void SynonymDictionary::_rightLeftRotation(NodeSynonymDictionary *& p_CriticalNode) 
   {
        _leftLeftRotation(p_CriticalNode->right);
        
        _rightRightRotation(p_CriticalNode);
   }
   

   SynonymDictionary::NodeSynonymDictionary* SynonymDictionary::_findRootWord(NodeSynonymDictionary* node, const std::string& word) const 
   {
        if (!node)
            return nullptr;
               
        if (word == node->rootWord || std::find(node->inflections.begin(), node->inflections.end(), word) != node->inflections.end())
            return node;

        if (word < node->rootWord)
            return _findRootWord(node->left, word);

        return _findRootWord(node->right, word);
   }
    

   void SynonymDictionary::_traverseTree(NodeSynonymDictionary* node, std::vector<NodeSynonymDictionary*>& nodes) const 
   {
        if (node != nullptr) 
          {
            _traverseTree(node->left, nodes);    

            nodes.push_back(node);              

            _traverseTree(node->right, nodes);      
          }
   }

   
   std::vector<SynonymDictionary::NodeSynonymDictionary*> SynonymDictionary::traverseTree() const 
   {
        std::vector<NodeSynonymDictionary*> nodes;
        
        _traverseTree(root, nodes);
        
        return nodes;
   }
   

   void SynonymDictionary::_removeSubtree(NodeSynonymDictionary*& node) 
   {
        if (node != nullptr) 
          {
            _removeSubtree(node->left);

            _removeSubtree(node->right);
            
            delete node;
            node = nullptr;
          }
   }

   void SynonymDictionary::_removeRootWordFromAVL(NodeSynonymDictionary*& node, const std::string& rootWord) 
   {
        if (rootWord < node->rootWord) 
          {
            _removeRootWordFromAVL(node->left, rootWord);
          } 
        
        else if (node->rootWord < rootWord) 
          {
            _removeRootWordFromAVL(node->right, rootWord);
          } 
        else 
          {
            if (node->left != nullptr && node->right != nullptr) 
              {
                _removeRightMinSuccessorNode(node);
              } 
            else 
              { 
                NodeSynonymDictionary* oldNode = node;
                node = (node->left != nullptr) ? node->left : node->right;
                delete oldNode;
                --numberOfRootWords;
              }
          }

        if (node != nullptr) 
          {
            _balanceNode(node); 
          }
   }

   
   void SynonymDictionary::_removeRightMinSuccessorNode(NodeSynonymDictionary* node) 
   {
        NodeSynonymDictionary* temporary = node->right;
        
        NodeSynonymDictionary* parent = node;
        
        while (temporary->left != nullptr) {
            parent = temporary;
            temporary = temporary->left;
        }
        
        node->rootWord = temporary->rootWord; 
        node->inflections = temporary->inflections;
        node->rootWordBelongsToSynonymGroups = temporary->rootWordBelongsToSynonymGroups;
        
        if (parent->left == temporary) 
          {
            _removeRootWordFromAVL(parent->left, temporary->rootWord);
          } 
        else 
          {
            _removeRootWordFromAVL(parent->right, temporary->rootWord);
          }
   }
    
}
