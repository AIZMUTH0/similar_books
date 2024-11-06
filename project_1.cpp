#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include <mutex>
#include <stdlib.h>
#include <algorithm>
#include <iomanip> //input output manipulater(Table output).

namespace file_system = std::filesystem;
using namespace std;

// Classes - Start

// Book File Blue Print.
class ParentFile
{
public:
   // This class variables will be Filled And will be accesible.
   int fileNumber = 0;
   string fileName;
   string filePath;

   vector<string> words;
   vector<int> counts;
   float totalWords = 0;
   vector<float> normalizeWords;

   // temperary variables
   vector<string> word_i;
   map<string, int> unsorted_map;
   vector<pair<string, int>> sorted_word_and_count;

   // default constructor.
   ParentFile() {};
   // default destructor.
   ~ParentFile() {};

   // For Reciving : Required information(File : number , name and path) and Assigning.
   void fileDetails_input(int i, vector<string> Names, vector<string> Paths);

   // For step 1 : Reading File : Alphabet or Numeric characters and storing as capital letters Word in vector word_i.
   void requireWords();

   // For Step 2 : Removing Commonly occuring words.
   void RemovingCommon();

   // For Storing , words ,counts and calculated normal frequency and Top occuring words wise sorting.
   void storingInClass();
   // For Manully clearing data calling when class object is not in use.
   void clear()
   {
      fileNumber = 0;
      fileName.clear();
      filePath.clear();
      words.clear();
      normalizeWords.clear();
   }
};

// Step 4 Similarity Matrix : Creating
struct similarityMatrix
{
public:
   int i;
   int j;
   float frequency;
};

// Classes - End

// Functions - Declarations - Start

void bookDetails(vector<string> &file_name, vector<string> &file_access_paths, int &filesCount);

void blink(int s_i, int s_j, int i, int j);

// Step 3 Similaity Index :function:
void similarityIndex(int i, vector<string> &w_i, vector<float> &n_i, int j, vector<string> &w, vector<float> &n_j, vector<similarityMatrix> &matrix);

// similarity matrix, sorting function,Comparing part
auto compareMatrix = [](const auto &matrix_1, const auto &matrix_2)
{
   return matrix_1.frequency > matrix_2.frequency;
};

mutex mtxLock;
int steps_i = 64;
int steps_j = 2016;

// Step 1 :Top occuring words:sorting function,Comparing part
bool comapareSecond(pair<string, int> &a, pair<string, int> &b);

// Step 5 : Reporting : printing top ten pairs of text book
void report(vector<similarityMatrix> matrix, vector<string> Book);

// Function-Declarations-End

// Main Function-starts

int main()
{
   // Getting Require File Details and Storing in vectors
   vector<string> file_names;
   vector<string> file_access_paths;
   int filesCount = 0;
   int count = 0;

   // Finding data-Set
   bookDetails(file_names, file_access_paths, filesCount);

   // Step 4 : Similarity Matrix :Part: blank.
   vector<similarityMatrix> similarity_matrix;

   

   // Step 4 : Similarity Matrix, wise looping (Excluding self similarity)
   // Creating,Sending,Processing,Comparing,Storing,Deleting and Repeating , for 2080 times for Required information.
   for (int i = 0; i < filesCount; i++)
   {
      ParentFile *File_i = new ParentFile;

      File_i->fileDetails_input(i, file_names, file_access_paths);

      File_i->requireWords();
      File_i->RemovingCommon();
      File_i->storingInClass();
      steps_i--;

      for (int j = 0; j < filesCount; j++)
      {

         if (i < j)
         {

            ParentFile *File_j = new ParentFile;

            File_j->fileDetails_input(j, file_names, file_access_paths);

            // Step 1 :
            File_j->requireWords();
            // Step 2 :
            File_j->RemovingCommon();
            File_j->storingInClass();

            // Step 3 :
            similarityIndex(i, File_i->words, File_i->normalizeWords, j, File_j->words, File_j->normalizeWords, similarity_matrix);

            File_j->clear();
            delete File_j;
            steps_j--;
            // this thread will show ongoing process
            thread T1(blink, steps_i, steps_j, i, j);
            T1.detach();
            
         }

         else
         {
            // Step 4 :
            similarity_matrix.push_back({i, j, 0.0});
         }
      }

      File_i->clear();
      delete File_i;
      count++;
   }

   // Step 4 : Similarity Matrix : sorting.
   sort(begin(similarity_matrix), end(similarity_matrix), compareMatrix);

   // Step 5 : Reporting top 10 similar pairs of text books.
   report(similarity_matrix, file_names);

   return 0;
}

// Main - Function - End

//-------------------------------------------------------------------//

//-------------------------------------------------------------------//

// Class - Function - Definitions - Start

// Reciving and Assigning
void ParentFile::fileDetails_input(int i, vector<string> Names, vector<string> Paths)
{
   fileNumber = i;
   fileName = Names[fileNumber];
   filePath = Paths[fileNumber];
}

// Step 1 : Reading files : filtering required information (alphabetic and numeric characters) , storing capital letter, words in unsorted map.
void ParentFile::requireWords()
{
   ifstream in_file(filePath);
   if (in_file)
   {
      char c;
      string word;
      c = in_file.get();
      while (in_file.good())
      {
         // Matching with ASCII chart if character is alphebet or nuemeric , then storing in Vector (string)
         if ((int(c) >= 65 && int(c) <= 90) || (int(c) >= 97 && int(c) <= 122) || (int(c) >= 48 && int(c) <= 57))
         {
            char ch;

            if (islower(c))
            {
               ch = toupper(c);
               word += ch;
            }
            else
            {
               word += c;
            }
         }
         else
         {
            word_i.push_back(word);
            word.erase();
         }
         c = in_file.get();
      }
      in_file.close();
   }
   // Scaning : word_i vector for word and its reoccurance  and Storing : in Unsorted map
   for (auto iterator = word_i.begin(); iterator != word_i.end(); iterator++)
   {
      unsorted_map[*iterator]++;
   }
   totalWords = word_i.size();
   word_i.clear();
}
// Step 1 : Done.

//-------------------------------------------------------------------//

// Step 2 : Removing : commonly occuring words while storing in returnable variables of class ParentFile.
void ParentFile::RemovingCommon()
{

   vector<string> common = {"", " ", "A", "AFTER", "ALL", "AN", "AND", "ANY", "ARE", "AS", "AT", "BE", "BEEN", "BEING", "BUT", "BY", "COULD", "DID", "DO", "FOR", "FROM",
                            "GOOD", "GREAT", "HAD", "HAVE", "HE", "HER", "HIM", "HIS", "I", "IF", "IN", "INTO", "IS", "IT", "MADE", "MAN", "MAY", "ME", "MORE", "MOST", "MY", "NO", "NOT", "NOW",
                            "OF", "ON", "ONE", "OR", "OUR", "OUT", "OTHER", "S", "SAID", "SHE", "SHOULD", "SO", "SOME", "SUCH", "THAN", "THAT", "THE", "THEIR", "THEM", "THEN", "THERE",
                            "THESE", "THEY", "THIS", "TIME", "TO", "UP", "UPON", "VERY"
                                                                                 "WAS",
                            "WERE", "WE", "WHAT", "WHEN", "WHICH", "WHO", "WILL", "WITH", "WOULD", "YOU"};

   for (auto it : common)
   {
      unsorted_map.erase(it);
   }
}
// Step 2 : Done.

//-------------------------------------------------------------------//

// Class - Function - Definitions - End
// Function - Definitions - Start

// Step 3 : SimilarityIndex
void similarityIndex(int i, vector<string> &Words_i, vector<float> &normalizeFrequency_i, int j, vector<string> &Words_j, vector<float> &normalizeFrequency_j, vector<similarityMatrix> &matrix)
{
   float frequency = 0.00;
   for (int i = 0; i < 100; i++)
   {
      for (int j = 0; j < 100; j++)
      {
         if (Words_i[i] == Words_j[j])
         {
            frequency += normalizeFrequency_i[i] + normalizeFrequency_i[j];
         }
      }
   }
   matrix.push_back({i, j, frequency});
   frequency = 0.00;
   Words_j.clear();
   normalizeFrequency_j.clear();
}
// Step 3 : Done.

//-------------------------------------------------------------------//

// Step 4 : Similarity Matrix is in main Function.
// Step 4 : Done.

//-------------------------------------------------------------------//

// Step 5 : Reporting
void report(vector<similarityMatrix> matrix, vector<string> Book)
{
   const char separator = ' ';
   const int file_nameWidth = 40;
   const int stringWidth = 22;
   const int numberWidth = 3;
   const int file_numberWidth = 4;
   const int frequencyWidth = 15;
   const int BookWidth = 9;

   cout << "\r";
   system("CLS");
   cout << "\r";
   cout << endl;

   cout << "Top Ten Similar Pair Of Text Books Are : " << endl
        << endl;
   for (int i = 0; i < 10; i++)
   {
      cout << left << setw(numberWidth) << setfill(separator) << i + 1;
      cout << left << setw(BookWidth) << setfill(separator) << "Book no.";
      cout << left << setw(file_numberWidth) << setfill(separator) << matrix[i].i;
      cout << left << setw(file_nameWidth) << setfill(separator) << Book[matrix[i].i];
      cout << left << setw(4) << setfill(separator) << "and";
      cout << left << setw(BookWidth) << setfill(separator) << "Book no.";
      cout << left << setw(file_numberWidth) << setfill(separator) << matrix[i].j;
      cout << left << setw(file_nameWidth) << setfill(separator) << Book[matrix[i].j];
      cout << left << setw(stringWidth) << setfill(separator) << "Similarity value is :";
      cout << left << setw(frequencyWidth) << setfill(separator) << matrix[i].frequency;

      cout << endl;
   }
}
// Step 5 : Done.

//-------------------------------------------------------------------//

bool comapareSecond(pair<string, int> &a, pair<string, int> &b)
{
   return a.second > b.second;
}
void ParentFile::storingInClass()
{
   // Sorting : storing Unsorted map to another vector of pair for sorting with compareSecond function.
   for (auto iterator = unsorted_map.begin(); iterator != unsorted_map.end(); iterator++)
   {
      sorted_word_and_count.push_back(*iterator);
   }
   sort(sorted_word_and_count.begin(), sorted_word_and_count.end(), comapareSecond);
   for (auto it : sorted_word_and_count)
   {
      float normalizeWord = 1.00;
      normalizeWord = float(it.second / totalWords);
      words.push_back(it.first);
      counts.push_back(it.second);
      normalizeWords.push_back(normalizeWord);
   }
   // Clearing : for Reusablity
   unsorted_map.clear();
   sorted_word_and_count.clear();
}

// Data-set entries Storing

void bookDetails(vector<string> &file_names, vector<string> &file_access_paths, int &filesCount)
{

   if (file_system::exists("..\\similar_books\\Book-Txt\\Book-Txt"))
   {
      for (auto &entry : file_system::directory_iterator("..\\similar_books\\Book-Txt\\Book-Txt"))
      {
         file_access_paths.push_back(entry.path().string());
         file_names.push_back(entry.path().filename().string());
         filesCount++;
      }
   }
   else if (file_system::exists("..\\similar_books\\Book-Txt"))
   {
      for (auto &entry : file_system::directory_iterator("..\\similar_books\\Book-Txt"))
      {
         file_access_paths.push_back(entry.path().string());
         file_names.push_back(entry.path().filename().string());
         filesCount++;
      }
   }

   else
   {
      string inputlocation;
      inputlocation = file_system::current_path().string();
      for (auto &entry : file_system::directory_iterator(inputlocation))
      {
         file_access_paths.push_back(entry.path().string());
         file_names.push_back(entry.path().filename().string());
         filesCount++;
      }

      if (file_access_paths.size() > 5)
      {
      }
      else
      {
         inputlocation.clear();

         cerr << "Data Set not Found !!" << endl;
         cout << "Please Enter Data set location to continiue Or Enter 0 (zero) For Exit" << endl;

         cin >> inputlocation;

         if (inputlocation == "0")
         {
            exit(1);
         }
         else
         {
            for (auto &entry : file_system::directory_iterator(inputlocation))
            {
               file_access_paths.push_back(entry.path().string());
               file_names.push_back(entry.path().filename().string());
               filesCount++;
            }
         }
      }
   }
}

void blink(int steps_i, int steps_j, int i, int j)
{
   mtxLock;
   cout << "\r";
   
   system("CLS");
   cout << string(100, ' ');
   cout << flush;
   cout << "\r";
   cout << "Please wait it has this Steps : " << steps_i << "--" << steps_j << " Remaining. " << "Prcessing 2D similarity Matrix i-j : " << i << "-" << j;
   cout << "\r";
   mtxLock;
}

//-------------------------------------------------------------------//

// Functions - Definitions - End
