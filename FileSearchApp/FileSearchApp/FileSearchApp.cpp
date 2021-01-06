#include <windows.h>
#include <Lmcons.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>

using namespace std;
namespace fs = std::filesystem;

void printManchineDetails()
{
	TCHAR machineName[UNLEN + 1];
	DWORD size = UNLEN + 1;
	GetComputerName((TCHAR*)machineName, &size);
	wcout << "Machine name : " << machineName << "\n";

	TCHAR userName[UNLEN + 1];
	GetUserName((TCHAR*)userName, &size);
	wcout << "User name :" << userName << "\n";
}

void serachTextInFile(filesystem::path path, string str)
{
	ifstream file(path);
	string line;
	bool isMatch = false;
	while (getline(file, line))
	{
		if (line.find(str) != string::npos)
		{
			isMatch = true;
			break;
		}
	}
	cout << (isMatch ? "Match was found " : "No matches were found ") << path.generic_u8string() << endl;
}

void searchFiles(string filePath, string searchText, bool isSearchInSub)
{
	if (isSearchInSub)
	{
		for (auto& p : fs::recursive_directory_iterator(filePath))
		{
			if (!p.is_directory())
			serachTextInFile(p.path(), searchText);
		}
	}
	else
	{
		for (auto& p : fs::directory_iterator(filePath))
		{
			if (!p.is_directory())
			serachTextInFile(p.path(), searchText);
			
			
		}
	}
}

vector<string>  splitString(string  str, char delim)
{
	size_t start;
	size_t end = 0;
	vector<string> folderpaths;
	while ((start = str.find_first_not_of(delim, end)) != string::npos)
	{
		end = str.find(delim, start);
		folderpaths.push_back(str.substr(start, end - start));
	}
	return folderpaths;
}


int main(int argc, char* argv[]) {

	string folderPaths = argv[1];
	string serachText = argv[2];
	string inSubString = argv[3];
	transform(inSubString.begin(), inSubString.end(), inSubString.begin(), ::tolower);
	bool isSearchInSub = (inSubString == "true" || inSubString == "1")? 1: 0;
	bool isParamProvided = false;
	vector<string> vFolderpaths;

	cout << "Have you provided parameters to Program(1/0)?" << endl;
	cin >> isParamProvided;
	if (!isParamProvided)
	{
		cout << "Please provide parameters and run again." << endl;
		return 0;
	}

	cout << endl;
	try
	{
		printManchineDetails();

		cout << endl << "Search is begining..." << endl << endl;

		vFolderpaths = splitString(folderPaths, '|');
		for (int i = 0; i < vFolderpaths.size(); i++) {
			if (fs::exists(vFolderpaths[i]))
			{
				searchFiles(vFolderpaths[i], serachText, isSearchInSub);
			}
			else
			{
				cout << "Folder doesn't exist " << vFolderpaths[i] << endl;
			}
		}
	}
	catch (exception ex)
	{
		cout << "Exception occured please contact administrator" << endl;
	}
	return 0;
}