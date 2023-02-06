#define _CRT_SECURE_NO_DEPRECATE //Отключение непринятия fopen

#include <iostream>
#include <fstream>
#include <string>
#include "zlib.h"
#include <vector>
#include <algorithm>
#include <iterator>

#define CHUNK 512000

using namespace std; //Знаю, что так лучше не делать, но даный пример не очень сложен, и никаких последствий повлечь за собой такое действие не должно. 

std::string line;

int def(FILE* source, FILE* dest, int level)
{

	int ret, flush;
	unsigned have;
	z_stream strm;

	unsigned char* in;
	in = (unsigned char*)malloc(CHUNK); //Задаю in так, чтобы избежать ошибки C6262 с превышением стека
	//unsigned char in[CHUNK];
	unsigned char* out;
	out = (unsigned char*)malloc(CHUNK); //Задаю out так, чтобы избежать ошибки C6262 с превышением стека

	/* Инициализация состояния deflate */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	ret = deflateInit(&strm, level);
	if (ret != Z_OK)
		return ret;

	do {

		if (in != 0) { //Для устранения ошибки С6387 в текущей строке под номером 40

			strm.avail_in = fread(in, 1, CHUNK, source);

			if (ferror(source)) {
				(void)deflateEnd(&strm);
				return Z_ERRNO;
			}
		
		}

		flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
		strm.next_in = in;
		

		/* Запуск deflate() для обработки входных данных, пока не заполнится

		   выходной буфер, с завершением сжатия, когда все данные источника

		   будут прочитаны */
		do {

			strm.avail_out = CHUNK;
			strm.next_out = out;

			ret = deflate(&strm, flush);    /* нет плохого значения возврата */
			
			  have = CHUNK - strm.avail_out;
         if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
         }

		} while (strm.avail_out == 0);

		/* Завершение, когда были обработаны последние данные файла */
	} while (flush != Z_FINISH);

	/* Очистка и возврат из функции def() */
	(void)deflateEnd(&strm);
	return Z_OK;
}

void compr(FILE* i, FILE* o) { //Функция выбора степени сжатия.

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
	
	string in_file = name; //Имя входного файла
	string stats = in_file + ".stat.txt"; //Имя файла со статистикой
	//in_file = in_file + ".txt";
	const char* path = in_file.c_str(); //Имя входного файла
	string outf = path; outf = outf + ".z"; //Имя сжатого файла
	const char* fin = outf.c_str();//Имя сжатого файла

	ifstream input(path); //Инициализация работы с файлом

	string temp1; //Переменная пробегаемой строки

	int counter = 0; //Счётчик слов

	vector <string> words; //Вектор слов

	string common; //Общая строка пробега символов


	while (getline(input, temp1)) { //Пробегаю файл построчно для сбора статистики


		common = common + temp1; //Общая строка символов

		cout << temp1 << " Stroka " << endl;

		int ultimcounter = 0; //Счётчик букв в строке


		int pos1 = 0; //Индикатор позиции символа

		int count = 0;

		for (int i = 0; i < temp1.length(); i++) {


			for (int as = 65; as <= 90; as++) {

				if (temp1[i] == char(as)) { ultimcounter++; } //Узнаю, есть ли в данной строке хоть одна большая буква (вернее сколько букв)

			}

			for (int as = 97; as <= 122; as++) {

				if (temp1[i] == char(as)) { ultimcounter++; } //Узнаю, есть ли в данной строке хоть одна малая буква (вернее сколько букв)

			}

			for (int j = 0; j <= 64; j++) { //Ищу знаки препинания. Если знак есть, увеличиваю счётчик слов counter на 1

				if (temp1[i] == char(j)) {

					counter++; count++;

					if (pos1 == 0 && !temp1.substr(pos1, i - pos1).empty()) { //Если попал на знак препинания, выписываю в вектор слов слово

						words.push_back(temp1.substr(pos1, i - pos1));

					}
					else if (pos1 < size(temp1) && !temp1.substr(pos1, i - pos1).empty()) {

						words.push_back(temp1.substr(pos1 + 1, i - pos1 - 1));

					}

					pos1 = i; //Если попал на знак препинания, выписываю в вектор слов слово\\

					if (words[size(words) - 1] == "." || words[size(words) - 1] == "\n" || words[size(words) - 1].empty()) { words.pop_back(); }

					for (int d = 0; d <= 64; d++) {

						if (temp1[i + 1] == char(d)) {

							counter--;

						} //Дополнительный проход, отсеивающий серии знаков препинания. 

						//Если после текущего знака также следует знак препинания, счётчик уменьшается на 1.
					}

					//cout << "symbol: " << temp1[i] << endl;
				}
			}

			for (int k = 91; k <= 96; k++) { //Ищу знаки препинания. Если знак есть, увеличиваю счётчик слов на 1

				if (temp1[i] == char(k)) {

					counter++; count++;

					if (pos1 == 0 && !temp1.substr(pos1, i - pos1).empty()) { //Если попал на знак препинания, выписываю в вектор слов слово

						words.push_back(temp1.substr(pos1, i - pos1));

					}
					else if (pos1 < size(temp1) && !temp1.substr(pos1, i - pos1).empty()) {

						words.push_back(temp1.substr(pos1 + 1, i - pos1 - 1));

					}

					pos1 = i; //Если попал на знак препинания, выписываю в вектор слов слово\\

					for (int d = 91; d <= 96; d++) {

						if (temp1[i + 1] == char(d)) { counter--; } //Дополнительный проход, отсеивающий серии знаков препинания. 

						//Если после текущего знака также следует знак препинания, счётчик уменьшается на 1.
					}
					//cout << "symbol: " << temp1[i] << endl;
				}
			}

			for (int k = 123; k <= 127; k++) { //Ищу знаки препинания. Если знак есть, увеличиваю счётчик слов на 1

				if (temp1[i] == char(k)) {

					counter++; count++;

					if (pos1 == 0 && !temp1.substr(pos1, i - pos1).empty()) { //Если попал на знак препинания, выписываю в вектор слов слово

						words.push_back(temp1.substr(pos1, i - pos1));

					}
					else if (pos1 < size(temp1) && !temp1.substr(pos1, i - pos1).empty()) {

						words.push_back(temp1.substr(pos1 + 1, i - pos1 - 1));

					}

					pos1 = i; //Если попал на знак препинания, выписываю в вектор слов слово\\

					for (int d = 123; d <= 127; d++) {

						if (temp1[i + 1] == char(d)) { counter--; } //Дополнительный проход, отсеивающий серии знаков препинания. 

						//Если после текущего знака также следует знак препинания, счётчик уменьшается на 1.

					}
					//cout << "symbol: " << temp1[i] << endl;
				}
			}
			//cout << "COUNT: " << count << endl;
		}

		if (count == 0) {
			if (!temp1.substr(pos1 + 1, size(temp1) - pos1).empty()) { words.push_back(temp1.substr(pos1, size(temp1) - pos1)); } //Запись последнего слова строки}
		}
		else {
			if (!temp1.substr(pos1 + 1, size(temp1) - pos1).empty()) { words.push_back(temp1.substr(pos1 + 1, size(temp1) - pos1)); } //Запись последнего слова строки
		}

		if (ultimcounter != 0) { counter++; }

		//cout << endl << "V dannoy stroke znakov: " << counter << endl << endl;//Если в строке оказались буквы, но не оказалось разделителей, увеличиваю счётчик слов на 1.

	}

	cout << "syms:: " << common << endl;

	cout << "slov v faile : " << counter << endl << endl;

	vector <int> counts; //Инициализация вектора количеств слов

	vector <string> wrds_fin = words; //Инициализация отсортированного вектора слов

	int wrd_counter = 0;

	for (int i = 0; i < words.size(); i++) { //Подсчёт повторяющихся слов (установление количеств слов)

		wrd_counter = 0;

		for (int j = 0; j < words.size(); j++) {

			if (words[i] == words[j]) { wrd_counter++; }
		}
		counts.push_back(wrd_counter);//Подсчёт повторяющихся слов (установление количеств слов)
	}

	std::sort(wrds_fin.begin(), wrds_fin.end()); //Удаление повторяющихся слов строки
	auto last = std::unique(wrds_fin.begin(), wrds_fin.end());
	wrds_fin.erase(last, wrds_fin.end()); //Удаление повторяющихся слов строки\\

	//cout << left << setw(10) << "vec: ";

	vector <int> inds(wrds_fin.size()); //Присвоение количеств слов прореженному списку слов

	for (int i = 0; i < wrds_fin.size(); i++) {

		for (int j = 0; j < words.size(); j++) {

			if (wrds_fin[i] == words[j]) { inds[i] = counts[j]; }
		}
	} //Присвоение количеств слов прореженному списку слов\\

	vector <int> counts_sym; //Инициализация вектора количеств символов
	string sym_fin = common; //Инициализация прореженного вектора символов
	int sym_counter = 0;

	for (int i = 0; i < common.size(); i++) { //Подсчёт повторяющихся символов (установление количеств символов)

		sym_counter = 0;

		for (int j = 0; j < common.size(); j++) {

			if (common[i] == common[j]) { sym_counter++; }
		}
		counts_sym.push_back(sym_counter);
	} //Подсчёт повторяющихся символов (установление количеств символов)\\

	for (string::iterator it1 = sym_fin.begin(); it1 != sym_fin.end(); ++it1) { //Удаление дублирующихс символов строки.
		for (string::iterator it2 = it1 + 1; it2 != sym_fin.end(); ) {
			if (*it1 == *it2) {
				sym_fin.erase(it2);
			}
			else {
				++it2;
			}
		}
	} //Удаление дублирующихс символов строки.\\

	vector <int> inds_sym(sym_fin.size()); //Присвоение количеств символов прореженному списку символов

	for (int i = 0; i < sym_fin.size(); i++) {

		for (int j = 0; j < common.size(); j++) {

			if (sym_fin[i] == common[j]) { inds_sym[i] = counts_sym[j]; }
		}
	} //Присвоение количеств символов прореженному списку слов\\

	//cout << "symFIN:  " << sym_fin << endl;

	string path1 = path; //Создание строки для обработки вывода статистики.
	
	ofstream statics; statics.open(stats);

	if (path1.substr((path1.size() - 4), 4) == ".txt") { //Вывод статистики в зависимости от имени файла. 
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

	FILE* i; //Инициализация входного файла
	FILE* o; //Инициализация выходного файла

	i = fopen(path, "r+"); //Открытие входного файла
	o = fopen(fin, "w+t"); //Создание/пересоздание выходного файла
	
	compr(i, o); // Сжатие файла.

	return 0;

}