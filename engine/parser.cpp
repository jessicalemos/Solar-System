#include "headers/parser.h"
#include "headers/Point.h"

int readPointsFile(string filename, vector<Point*> *points) {
	string l, t, token;
	ifstream file(filename);
	int j, i = 0;
	vector<float> tokens;

	if (!file.is_open()) {
		cout << "Unable to open file: " << filename << "." << endl;
		return -1;
	}
	else
	{
		while (!file.eof())
		{
			getline(file, l);
			stringstream ss(l.c_str());

			if (l.c_str() != NULL)
			{
				j = 0;

				while(j < 3 && getline(ss,token,',')) {
					tokens.push_back(stof(token));
					j++;
				}

				Point *p = new Point(tokens[i++], tokens[i++], tokens[i++]);
				points->push_back(p);
			}
		}
		points->pop_back();
		file.close();
	}
	return 0;
}

int loadXMLfile(string filename, vector<Point*> *points) {
	XMLDocument xmlDoc;
    XMLNode *pRoot;
    XMLElement *pElement, *pListElement;
    string fileDir = "../../files/" + filename;
    XMLError eResult = xmlDoc.LoadFile(fileDir.c_str());
    
    if (eResult == XML_SUCCESS)
    {
        pRoot = xmlDoc.FirstChild();
        if (pRoot != nullptr)
        {
            pElement = pRoot->FirstChildElement("models");
            
            if (pElement != nullptr)
            {
                pListElement = pElement->FirstChildElement("model");
                
                while (pListElement != nullptr)
                {
                    string file;
                    file = pListElement->Attribute("file");
                    if (!file.empty() && readPointsFile(file,points) == -1)
                        return -1;

                    pListElement = pListElement->NextSiblingElement("model");
                }
            }
        }
    }
    else
    {
        cout << "Unable to open file: " << filename << "." << endl;
        return -1;
    }
    return 0;
}