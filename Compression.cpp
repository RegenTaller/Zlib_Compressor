#define _CRT_SECURE_NO_DEPRECATE //���������� ���������� fopen

#include <iostream>
#include <fstream>
#include <string>
#include "zlib.h"
#include <vector>
#include <algorithm>
#include <iterator>

#define CHUNK 512000

using namespace std; //����, ��� ��� ����� �� ������, �� ����� ������ �� ����� ������, � ������� ����������� ������� �� ����� ����� �������� �� ������. 

std::string line;

int def(FILE* source, FILE* dest, int level)
{

	int ret, flush;
	unsigned have;
	z_stream strm;

	unsigned char* in;
	in = (unsigned char*)malloc(CHUNK); //����� in ���, ����� �������� ������ C6262 � ����������� �����
	//unsigned char in[CHUNK];
	unsigned char* out;
	out = (unsigned char*)malloc(CHUNK); //����� out ���, ����� �������� ������ C6262 � ����������� �����

	/* ������������� ��������� deflate */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	ret = deflateInit(&strm, level);
	if (ret != Z_OK)
		return ret;

	do {

		if (in != 0) { //��� ���������� ������ �6387 � ������� ������ ��� ������� 40

			strm.avail_in = fread(in, 1, CHUNK, source);

			if (ferror(source)) {
				(void)deflateEnd(&strm);
				return Z_ERRNO;
			}
		
		}

		flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
		strm.next_in = in;
		

		/* ������ deflate() ��� ��������� ������� ������, ���� �� ����������

		   �������� �����, � ����������� ������, ����� ��� ������ ���������

		   ����� ��������� */
		do {

			strm.avail_out = CHUNK;
			strm.next_out = out;

			ret = deflate(&strm, flush);    /* ��� ������� �������� �������� */
			
			  have = CHUNK - strm.avail_out;
         if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
         }

		} while (strm.avail_out == 0);

		/* ����������, ����� ���� ���������� ��������� ������ ����� */
	} while (flush != Z_FINISH);

	/* ������� � ������� �� ������� def() */
	(void)deflateEnd(&strm);
	return Z_OK;
}

void compr(FILE* i, FILE* o) { //������� ������ ������� ������.

	cout << endl << "Enter compression value (1-9; 1 is recommended for light files): "; int comp = 1; cin >> comp;

	switch (comp) {
	case 1: def(i, o, comp); break;
	case 2: def(i, o, comp); break;
	case 3: def(i, o, comp); break;
	case 4: def(i, o, comp); break;
	case 5: def(i, o, comp); break;
	case 6: def(i, o, comp); break;
	case 7: def(i, o, comp); break;
	case 8: def(i, o, comp); break;
	case 9: def(i, o, comp); break;
	default:
		cout << "Such value is not available for compression value, try again:\n";
		compr(i, o);
		break;
	}
}
	
int main() {

	const char* name = "Example.t";
	
	string in_file = name; //��� �������� �����
	string stats = in_file + ".stat.txt"; //��� ����� �� �����������
	//in_file = in_file + ".txt";
	const char* path = in_file.c_str(); //��� �������� �����
	string outf = path; outf = outf + ".z"; //��� ������� �����
	const char* fin = outf.c_str();//��� ������� �����

	ifstream input(path); //������������� ������ � ������

	string temp1; //���������� ����������� ������

	int counter = 0; //������� ����

	vector <string> words; //������ ����

	string common; //����� ������ ������� ��������


	while (getline(input, temp1)) { //�������� ���� ��������� ��� ����� ����������


		common = common + temp1; //����� ������ ��������

		cout << temp1 << " Stroka " << endl;

		int ultimcounter = 0; //������� ���� � ������


		int pos1 = 0; //��������� ������� �������

		int count = 0;

		for (int i = 0; i < temp1.length(); i++) {


			for (int as = 65; as <= 90; as++) {

				if (temp1[i] == char(as)) { ultimcounter++; } //�����, ���� �� � ������ ������ ���� ���� ������� ����� (������ ������� ����)

			}

			for (int as = 97; as <= 122; as++) {

				if (temp1[i] == char(as)) { ultimcounter++; } //�����, ���� �� � ������ ������ ���� ���� ����� ����� (������ ������� ����)

			}

			for (int j = 0; j <= 64; j++) { //��� ����� ����������. ���� ���� ����, ���������� ������� ���� counter �� 1

				if (temp1[i] == char(j)) {

					counter++; count++;

					if (pos1 == 0 && !temp1.substr(pos1, i - pos1).empty()) { //���� ����� �� ���� ����������, ��������� � ������ ���� �����

						words.push_back(temp1.substr(pos1, i - pos1));

					}
					else if (pos1 < size(temp1) && !temp1.substr(pos1, i - pos1).empty()) {

						words.push_back(temp1.substr(pos1 + 1, i - pos1 - 1));

					}

					pos1 = i; //���� ����� �� ���� ����������, ��������� � ������ ���� �����\\

					if (words[size(words) - 1] == "." || words[size(words) - 1] == "\n" || words[size(words) - 1].empty()) { words.pop_back(); }

					for (int d = 0; d <= 64; d++) {

						if (temp1[i + 1] == char(d)) {

							counter--;

						} //�������������� ������, ����������� ����� ������ ����������. 

						//���� ����� �������� ����� ����� ������� ���� ����������, ������� ����������� �� 1.
					}

					//cout << "symbol: " << temp1[i] << endl;
				}
			}

			for (int k = 91; k <= 96; k++) { //��� ����� ����������. ���� ���� ����, ���������� ������� ���� �� 1

				if (temp1[i] == char(k)) {

					counter++; count++;

					if (pos1 == 0 && !temp1.substr(pos1, i - pos1).empty()) { //���� ����� �� ���� ����������, ��������� � ������ ���� �����

						words.push_back(temp1.substr(pos1, i - pos1));

					}
					else if (pos1 < size(temp1) && !temp1.substr(pos1, i - pos1).empty()) {

						words.push_back(temp1.substr(pos1 + 1, i - pos1 - 1));

					}

					pos1 = i; //���� ����� �� ���� ����������, ��������� � ������ ���� �����\\

					for (int d = 91; d <= 96; d++) {

						if (temp1[i + 1] == char(d)) { counter--; } //�������������� ������, ����������� ����� ������ ����������. 

						//���� ����� �������� ����� ����� ������� ���� ����������, ������� ����������� �� 1.
					}
					//cout << "symbol: " << temp1[i] << endl;
				}
			}

			for (int k = 123; k <= 127; k++) { //��� ����� ����������. ���� ���� ����, ���������� ������� ���� �� 1

				if (temp1[i] == char(k)) {

					counter++; count++;

					if (pos1 == 0 && !temp1.substr(pos1, i - pos1).empty()) { //���� ����� �� ���� ����������, ��������� � ������ ���� �����

						words.push_back(temp1.substr(pos1, i - pos1));

					}
					else if (pos1 < size(temp1) && !temp1.substr(pos1, i - pos1).empty()) {

						words.push_back(temp1.substr(pos1 + 1, i - pos1 - 1));

					}

					pos1 = i; //���� ����� �� ���� ����������, ��������� � ������ ���� �����\\

					for (int d = 123; d <= 127; d++) {

						if (temp1[i + 1] == char(d)) { counter--; } //�������������� ������, ����������� ����� ������ ����������. 

						//���� ����� �������� ����� ����� ������� ���� ����������, ������� ����������� �� 1.

					}
					//cout << "symbol: " << temp1[i] << endl;
				}
			}
			//cout << "COUNT: " << count << endl;
		}

		if (count == 0) {
			if (!temp1.substr(pos1 + 1, size(temp1) - pos1).empty()) { words.push_back(temp1.substr(pos1, size(temp1) - pos1)); } //������ ���������� ����� ������}
		}
		else {
			if (!temp1.substr(pos1 + 1, size(temp1) - pos1).empty()) { words.push_back(temp1.substr(pos1 + 1, size(temp1) - pos1)); } //������ ���������� ����� ������
		}

		if (ultimcounter != 0) { counter++; }

		//cout << endl << "V dannoy stroke znakov: " << counter << endl << endl;//���� � ������ ��������� �����, �� �� ��������� ������������, ���������� ������� ���� �� 1.

	}

	cout << "syms:: " << common << endl;

	cout << "slov v faile : " << counter << endl << endl;

	vector <int> counts; //������������� ������� ��������� ����

	vector <string> wrds_fin = words; //������������� ���������������� ������� ����

	int wrd_counter = 0;

	for (int i = 0; i < words.size(); i++) { //������� ������������� ���� (������������ ��������� ����)

		wrd_counter = 0;

		for (int j = 0; j < words.size(); j++) {

			if (words[i] == words[j]) { wrd_counter++; }
		}
		counts.push_back(wrd_counter);//������� ������������� ���� (������������ ��������� ����)
	}

	std::sort(wrds_fin.begin(), wrds_fin.end()); //�������� ������������� ���� ������
	auto last = std::unique(wrds_fin.begin(), wrds_fin.end());
	wrds_fin.erase(last, wrds_fin.end()); //�������� ������������� ���� ������\\

	//cout << left << setw(10) << "vec: ";

	vector <int> inds(wrds_fin.size()); //���������� ��������� ���� ������������ ������ ����

	for (int i = 0; i < wrds_fin.size(); i++) {

		for (int j = 0; j < words.size(); j++) {

			if (wrds_fin[i] == words[j]) { inds[i] = counts[j]; }
		}
	} //���������� ��������� ���� ������������ ������ ����\\

	vector <int> counts_sym; //������������� ������� ��������� ��������
	string sym_fin = common; //������������� ������������ ������� ��������
	int sym_counter = 0;

	for (int i = 0; i < common.size(); i++) { //������� ������������� �������� (������������ ��������� ��������)

		sym_counter = 0;

		for (int j = 0; j < common.size(); j++) {

			if (common[i] == common[j]) { sym_counter++; }
		}
		counts_sym.push_back(sym_counter);
	} //������� ������������� �������� (������������ ��������� ��������)\\

	for (string::iterator it1 = sym_fin.begin(); it1 != sym_fin.end(); ++it1) { //�������� ������������ �������� ������.
		for (string::iterator it2 = it1 + 1; it2 != sym_fin.end(); ) {
			if (*it1 == *it2) {
				sym_fin.erase(it2);
			}
			else {
				++it2;
			}
		}
	} //�������� ������������ �������� ������.\\

	vector <int> inds_sym(sym_fin.size()); //���������� ��������� �������� ������������ ������ ��������

	for (int i = 0; i < sym_fin.size(); i++) {

		for (int j = 0; j < common.size(); j++) {

			if (sym_fin[i] == common[j]) { inds_sym[i] = counts_sym[j]; }
		}
	} //���������� ��������� �������� ������������ ������ ����\\

	//cout << "symFIN:  " << sym_fin << endl;

	string path1 = path; //�������� ������ ��� ��������� ������ ����������.
	
	ofstream statics; statics.open(stats);

	if (path1.substr((path1.size() - 4), 4) == ".txt") { //����� ���������� � ����������� �� ����� �����. 
		char c = '"';
		for (int i = 0; i < size(wrds_fin); i++) { 
			cout << "WORD " << c << wrds_fin[i] << c << ":" << inds[i] << endl; 
			statics << "WORD " << c << wrds_fin[i] << c << ":" << inds[i] << endl;
		}
	}
	else {
		string s = "'";
		for (int i = 0; i < size(sym_fin); i++) {
			char c[20]; _itoa(sym_fin[i], c, 16);
			cout << "SYMBOL " << s << sym_fin[i] << s << ": ASCII CODE=(" << c << ", " << (int)sym_fin[i] << ") COUNT=" << (inds_sym[i]) << endl;
			statics << "SYMBOL " << s << sym_fin[i] << s << ": ASCII CODE=(" << c << ", " << (int)sym_fin[i] << ") COUNT=" << (inds_sym[i]) << endl;
		}
	}

	FILE* i; //������������� �������� �����
	FILE* o; //������������� ��������� �����

	i = fopen(path, "r+"); //�������� �������� �����
	o = fopen(fin, "w+t"); //��������/������������ ��������� �����
	
	compr(i, o); // ������ �����.

	return 0;

}