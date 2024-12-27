/**
* \file main.cpp
* \brief Test File 
*/

#include "SynonymDictionary.h"
using namespace std;
using namespace synonymDictionaryUtils;

int main()
{
	int nb = 1;				
	string rootWord;			
	string inflection;			
	string synonym;		
	SynonymDictionary testDictionary;	
	SynonymDictionary testDictionaryFromFileTxt;	
	ifstream InputFile;	


	cout << "Welcome to your dictionary of synonyms" << endl;
	
	cout << "----------------------- Menu Options -----------------------" << endl;
	cout << "Option 1 - Add a root word to the dictionary." << endl;
	cout << "Option 2 - Add an inflection to a root word." << endl;
	cout << "Option 3 - Add a synonym to a root word." << endl;
	cout << "Option 4 - Remove an inflection from a root word." << endl;
	cout << "Option 5 - Remove a synonym from a root word." << endl;
	cout << "Option 6 - Remove a root word from the dictionary." << endl;
	cout << "Option 7 - Load a dictionary from a txt file" << endl;
	cout << "Option 8 - Modify a chain of character using the dictionary." << endl;
//	cout << "Option 9 - Transform a custom sentence" << endl;
	cout << "Option 0 - Quit." << endl;
	cout << "------------------------------------------------------------" << endl;

	while (nb != 0)
	{
		do
		{
			cout << endl;
			cout << "Please select an option 0-8" << endl;
			cin >> nb;
			if(nb <0 || nb>9)
			{
				cout << "***Invalid Option***\n";
			}
		}while(nb <0 || nb>9);

		try
		{
			switch (nb)
			{

			case 1: 
			{
				cout << "Enter the root word you want to add: " << endl;
				cin >> rootWord;
				testDictionary.addRootWord(rootWord);
				cout << "The root word '" << rootWord << "' has been added!" << endl;
				cout << "Display of dictionary: " << endl << testDictionary << endl;
				break;
			}

			case 2: 
			{
				cout << "Enter the rootWord to which you want to add an inflection to: " << endl;
				cin >> rootWord;
				cout << "Enter the inflection to add for this root word: " << endl;
				cin >> inflection;
				testDictionary.addInflection(rootWord, inflection);
				cout << "The inflection '" << inflection << "' has been added to the root word '" << rootWord << "' !" << endl;
				cout << "Display of dictionary: " << endl << testDictionary << endl;
				break;
			}

			case 3: 
			{
				cout << "Enter the root word for which you want to add a synonym: " << endl;
				cin >> rootWord;
				cout << "Enter the synonym to add for this root word:" << endl;
				cin >> synonym;
				int position = -1;  //Insérer un nouveau synonym, n'oubliez pas de tester les autres cas !
				testDictionary.addSynonym(rootWord, synonym, position);
				cout << "The synonym '" << synonym << "'has been added to the root word '" << rootWord << "' !" << endl;
				cout << "Display of dictionary: " << endl << testDictionary << endl;
				break;
			}

			case 4: 
			{
				cout << "Enter the root word from which you want to remove an inflection:" << endl;
				cin >> rootWord;
				cout << "Enter the inflection to remove:" << endl;
				cin >> inflection;
				testDictionary.removeInflection(rootWord, inflection);
				cout << "The inflection '" << inflection << "' has been removed from the root word '" << rootWord << "' !" << endl;
				cout << "Display of dictionary: " << endl << testDictionary << endl;
				break;
			}

			case 5: 
			{
				cout << "Enter the root word from which you want to remove a synonym:" << endl;
				cin >> rootWord;
				cout << "What is the synonym you want to remove?" << endl;
				cin >> synonym;
				int position = 0; //Enlevez le premier synonym, n'oubliez pas de tester les autres cas !
				testDictionary.removeSynonym(rootWord, synonym, position);
				cout << "The synonym '" << synonym << "' has been removed from the root word '" << rootWord << "' !" << endl;
				cout << "Display of dictionary: " << endl << testDictionary << endl;
				break;
			}

			case 6: 
			{
				cout << "Enter the root word to remove:" << endl;
				cin >> rootWord;
                                cout << "The root word '" << rootWord << "' has been removed!" << endl;
				testDictionary.removeRootWord(rootWord);				
				cout << "Display of dictionary: " << endl << testDictionary << endl;
				break;
			}

			case 7: 
			{
                                
				InputFile.open("PetitDico.txt", ios::in);
				if(InputFile.is_open())
				{
					testDictionaryFromFileTxt.loadSynonymDictionary(InputFile);
				}
				InputFile.close();
				cout << "The dictionary is loaded !" << endl;
				cout << "Display of dictionary: " << endl << testDictionaryFromFileTxt << endl;
				break;
			}

			case 8: 
			{
				string chain = "Nous poursuivrons nos etudes plus lentement";
				cout << "Sentence to transform: " << chain << endl;
				string result = "";
				istringstream iss(chain);
				string word;
				while (iss >> word)
				{
					try
					{
						rootWord = testDictionaryFromFileTxt.findRootWord(word);
						cout << "Root word found : " << rootWord << endl
								<< "To change the word '" << word << "', enter o: ";
						char repch;
						cin >> repch;
						if (repch != 'o')
						{
							cout << "The word " << word << " will not be changed" << endl;
							result += (word + " ");
						}
						else
						{
							int numberOfMeaning = testDictionaryFromFileTxt.getNumberOfMeanings(rootWord);
							int repin = 1;
							if (numberOfMeaning > 1)
							{
								for (int j = 0; j < numberOfMeaning; j++)
								{
									cout << "Meaning #" << j + 1 << " : ";
									cout << testDictionaryFromFileTxt.getMeaning(rootWord, j) << endl;
								}
								cout << "Which meaning would you like:? ";
								cin >> repin;
							}
							std::vector<std::string> synonyms = testDictionaryFromFileTxt.getSynonyms(rootWord, repin-1);
							int nbSynonymes = 0;
							for (vector<string>::const_iterator it = synonyms.begin(); it != synonyms.end(); ++it)
							{
								nbSynonymes++;
								cout << "Synonym #" << nbSynonymes << ": " << *it << endl;
							}
							cout << "Which synonym do you choose:? ";
							cin >> repin;
							vector<string>::const_iterator it2 = synonyms.begin();
							for (int k = 0; k != repin - 1; k++)
								it2++;
							std::vector<std::string> inflections = testDictionaryFromFileTxt.getInflections(*it2);
							int nbFlexions = 0;
							for (vector<string>::const_iterator it = inflections.begin(); it != inflections.end(); ++it)
							{
								nbFlexions++;
								cout << "Inflection #" << nbFlexions << ": " << *it << endl;
							}
							cout << "Which inflection do you choose?: ";
							cin >> repin;
							it2 = inflections.begin();
							for (int l = 0; l != repin - 1; l++)
								it2++;
							result += (*it2 + " ");
						}
					}
					catch (std::logic_error& e)
					{
						result += (word + " ");
						word.clear();
						continue;
					}
				}
				cout << "Here is the transformed text: " << endl;
				cout << result << endl;
				break;
			}
			
//			case 9: //Check if AVL Tree is balanced
//			{
//				if(testDictionaryFromFileTxt.isAVLTree())
//					cout << "The tree is balanced !" << endl;
//				else
//					cout << "The tree is inbalanced !" << endl;
//				break;
//			}
                        
//                        case 9: 
//                        {
//                            cout << "Please enter the sentence you want to transform: " << endl;
//                            cin.ignore(); 
//                            string userChain;
//                            getline(cin, userChain); 
//
//                            cout << "Sentence to transform: " << userChain << endl;
//                            string result = "";
//                            istringstream iss(userChain);
//                            string word;
//
//                            while (iss >> word)
//                            {
//                                try
//                                {                                    
//                                    rootWord = testDictionaryFromFileTxt.findRootWord(word);
//                                    cout << "Root word found: " << rootWord << endl;
//                                    cout << "To change the word '" << word << "', enter 'o': ";
//
//                                    char repch;
//                                    cin >> repch;
//                                    if (repch != 'o')
//                                    {
//                                        cout << "The word " << word << " will not be changed" << endl;
//                                        result += (word + " ");
//                                    }
//                                    else
//                                    {                                       
//                                        int numberOfMeaning = testDictionaryFromFileTxt.getNumberOfMeanings(rootWord);
//                                        int repin = 1;
//
//                                        if (numberOfMeaning > 1)
//                                        {
//                                            for (int j = 0; j < numberOfMeaning; j++)
//                                            {
//                                                cout << "Meaning #" << j + 1 << " : " << testDictionaryFromFileTxt.getMeaning(rootWord, j) << endl;
//                                            }
//                                            cout << "Which meaning would you like? (Enter a number): ";
//                                            cin >> repin;
//                                        }
//                                      
//                                        std::vector<std::string> synonyms = testDictionaryFromFileTxt.getSynonyms(rootWord, repin - 1);
//                                        int nbSynonymes = 0;
//                                        for (vector<string>::const_iterator it = synonyms.begin(); it != synonyms.end(); ++it)
//                                        {
//                                            nbSynonymes++;
//                                            cout << "Synonym #" << nbSynonymes << ": " << *it << endl;
//                                        }
//                                      
//                                        if (nbSynonymes == 0)
//                                        {
//                                            cout << "No synonyms found for this word." << endl;
//                                            result += (word + " ");
//                                            continue; // Skip the current word
//                                        }
//
//                                        cout << "Which synonym do you choose? (Enter a number): ";
//                                        cin >> repin;
//                                      
//                                        vector<string>::const_iterator it2 = synonyms.begin();
//                                        for (int k = 0; k != repin - 1; k++)
//                                            it2++;
//                                       
//                                        std::vector<std::string> inflections = testDictionaryFromFileTxt.getInflections(*it2);
//                                        int nbFlexions = 0;
//                                        for (vector<string>::const_iterator it = inflections.begin(); it != inflections.end(); ++it)
//                                        {
//                                            nbFlexions++;
//                                            cout << "Inflection #" << nbFlexions << ": " << *it << endl;
//                                        }
//                                       
//                                        if (nbFlexions == 0)
//                                        {
//                                            cout << "No inflections found for this synonym." << endl;
//                                            result += (*it2 + " ");
//                                            continue; // Skip the current word
//                                        }
//
//                                        cout << "Which inflection do you choose? (Enter a number): ";
//                                        cin >> repin;
//                                       
//                                        it2 = inflections.begin();
//                                        for (int l = 0; l != repin - 1; l++)
//                                            it2++;
//
//                                        result += (*it2 + " ");
//                                    }
//                                }
//                                catch (std::logic_error& e)
//                                {
//                                    result += (word + " ");
//                                    word.clear();
//                                    continue;
//                                }
//                            }
//                            
//                            cout << "Here is the transformed text: " << endl;
//                            cout << result << endl;
//                            break;
//                        }

			}
		}

		catch(exception & e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl;
		}
	}

	cout << "Goodbye !" << endl;
	return 0;
}
