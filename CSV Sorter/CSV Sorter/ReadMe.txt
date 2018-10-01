Partners: Stephen Quickel- svq3  Masood Karimi- mkk102

Functions:
  void checkNumeric(char* columnName);
  
  int checkForColumn(char* check);
  
  long int findSize(char file_name[]);
  
  int getSize(char *array);

  char* getFirstValue(char* entry, int startIndex);
  
  int cmpString(char* str1, char* str2);
  
  int cmpInt(int firstInteger, int secondInteger);
  
  void printCSV(csventry* entries, int rows, csventry* columnArr);
  
  int main(int varc, char* argv[]);
MergeSort Implementation:
  int cmpString(char* str1, char* str2);
  
  int cmpInt(int firstInteger, int secondInteger);
  
  void merge(csventry* entryArr, int low, int mid, int high, int colID, int numeric);
  
  void mergesorts(csventry* entryArr, int low, int high, int colID, int numeric);


Predicted Run time: 

  Given that all the reads and print statement to the CSV file would be O(1) merge sort would be the defining funtion for this programs
  which would be O(nlogn).

