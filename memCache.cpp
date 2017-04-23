#include <iostream>
#include <map>
#include <stdio.h>
#include <fstream>
#include <list>

using namespace std;


int main()
{
	string *array;
	// list record
	list<string> record;
	string page;
	int num;
	long miss, hit;
	double ratio;
	map<string, list<string>::iterator> m;
	map<string, int> m_fifo;
	ifstream fin("trace.txt");

	cout<<"FIFO---"<<endl;
	cout<<"size	miss	hit	page fault ratio"<<endl;
	for(int i=64;i<=512;i*=2){
		num=0;
		miss=0, hit=0;
		array = new string[i];	
		m_fifo.clear();
		while(getline(fin, page)){
			page = page.substr(3,5);
			if(m_fifo.find(page) != m_fifo.end()){  // hit
				hit++;
			}else{  //miss
				miss++;
				if(num < i){   //not full
					m_fifo[page] = num;
					array[num] = page;
					num++;
				}else{	//full
					m_fifo.erase(array[num%i]);
					m_fifo[page] = num%i;
					array[num%i] = page;
					num++;
				}
			}
		}

		ratio = miss*1.0 / (miss+hit);
		printf("%d	%ld	%ld	%lf\n", i, miss, hit, ratio);

		fin.clear();                 // clear fail and eof bits
		fin.seekg(0, std::ios::beg); // back to the start!
		delete [] array;
	}




	cout<<"LRU---"<<endl;
	cout<<"size	miss	hit	page fault ratio"<<endl;
	for(int i=64;i<=512;i*=2){
//		num=0;
		miss=0, hit=0;
//		array = new string[i];
		m.clear();
		record.clear();
		ifstream fin("trace.txt");
		while(getline(fin, page)){
			page = page.substr(3,5);
			if(m.find(page) != m.end()){  // hit
				//int tnum = num<i ? num-1:i-1; 
				//int ptr = m[page];
				//for(int j=ptr;j<tnum;j++){
				//	array[j] = array[j+1];
				//	m[array[j+1]]=j;
				//}
				//array[tnum]=page; 
				//m[page] = tnum;  //hit node

				record.erase(m[page]);
				record.push_front(page);
				m[page] = record.begin();
				hit++;
			}else{  //miss
				miss++;
				if(record.size() < i){   //not fullp
					record.push_front(page);
					m[page] = record.begin();
					//array[num] = page;
					//num++;
				}else{	//full
					m.erase(*(--record.end()));
					/*
					for(int j=0;j<i-1;j++){
						array[j] = array[j+1];
						m[array[j+1]]=j;
					}
					m[page] = i-1;
					array[i-1] = page;
					*/
					record.erase(--record.end());
					record.push_front(page);
					m[page] = record.begin();
				}
			}
		}

		ratio = miss*1.0 / (miss+hit);
		printf("%d	%ld	%ld	%lf\n", i, miss, hit, ratio);

		fin.close();	
		//fin.seekg(0, std::ios::beg); // back to the start!
//		delete [] array;
	}

	//fin.close();	
}
