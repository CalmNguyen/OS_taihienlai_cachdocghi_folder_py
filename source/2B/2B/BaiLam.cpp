#include"volume.h"
#include"BaiLam.h"
int changePW() {
	char* x = new char[512];
	string s= readSector(1, 0, x);
	string pw = "";
	string pw1 = "";
	string old_pw = s.substr(22, 8);
	if (s[21] == '0')
	{
		while (pw.length() < 8 | pw != pw1) {
			cout << "Nhap mat khau 8 ky tu:";
			cin >> pw;
			cout << "Xac nhan lai mat khau:";
			cin >> pw1;
			if (pw.length() < 8)
				cout << "Mat khau khong dung kich thuoc\n";
			if (pw != pw1) {
				cout << "Mat khau khong dong nhat\n";
			}
		}
		s[21] = '1';
		s.replace(22, 8, pw);
		writeSector(s, 1, 0);
		return 1;
	}
	else
	{
		while (1) {
			cout << "Nhap mat khau cu:";
			cin >> pw;
			if (pw == old_pw) {
				while (pw.length() < 8 | pw != pw1) {
					cout << "Nhap mat khau 8 ky tu:";
					cin >> pw;
					cout << "Xac nhan lai mat khau:";
					cin >> pw1;
					if (pw.length() < 8)
						cout << "Mat khau khong dung kich thuoc\n";
					if (pw != pw1) {
						cout << "Mat khau khong dong nhat\n";
					}
				}
			}
			else {
				cout << "Mat khau cu khong trung khop\n";
			}
			s.replace(22, 8, pw);
			writeSector(s, 1, 0);
			return 1;
		}
	}
	return 0;
}
//bang RDET: 8 bytes dau ghi tong so entry
// 52B 1 entry
//bang RDET chiem 4 block 
int get_size_RDET() {
	char* x = new char[4 * 2 * 512];
	string s = readBlock(4, 3, x);
	int size = StranToDe(s.substr(0, 8), 10);
	return size;
}
TapTin* readRDET() {
	int size = get_size_RDET();
	char* x = new char[512*4*2];
	string s = readBlock(4, 3, x);
	TapTin* tt = new TapTin[size];
	for (int i = 0; i < size; i++) {
		tt[i].doc(s, 8 + 52 * i);
	}
	return tt;
}
void addFile_RDET(TapTin add) {
	char* x = new char[4 * 2 * 512];
	string s = readBlock(4, 3, x);
	int size = get_size_RDET();
	s = add.ghi(s, 8 + size * 52);
	size++;
	string temp = TransToOptions(size, 10);
	s.replace(0 + 8 - temp.length(), temp.length(), temp);
	writeBlock(s, 4, 3);
}
void listFile() {
	TapTin* x = new TapTin[get_size_RDET()];
	x = readRDET();
	cout << "stt\tTen file\tKich thuoc\tChe do bao mat\tblock bat dau\n";
	for (int i = 0; i < get_size_RDET(); i++) {
		x[i].xuat();
	}
}
bool CheckName(string name, int &vi_tri) {
	if (get_size_RDET() == 0)
		return true;
	TapTin* x = new TapTin[get_size_RDET()];
	x = readRDET();
	string mo_rong = name.substr(name.length() - 3, 3);
	string nf = name.replace(name.length() - 4, 4, "");
	if (nf.length() < 16) {
		string xx = "";
		for (int i = 0; i < 16 - nf.length(); i++)
			xx += ' ';
		nf = xx + nf;
	}
	for (int i = 0; i < get_size_RDET(); i++) {
		if (nf == x[i].TenTapTin && mo_rong == x[i].PhanMoRong)
		{
			vi_tri = i;
			return false;
		}
	}
	return true;
}
bool Import(string name) {
	int o;
	if (!CheckName(name,o)) {
		cout << "Ten file da ton tai\n";
		return false;
	}
	ifstream out;
	out.open(name, ios::binary | ios::out);
	string temp = "";
	string all = "";
	while (!out.eof()) {
		getline(out, temp);
		all += temp;
		if (!out.eof())
			all += '\n';
	}
	int size = all.length();
	int nclu = size / 1024;
	if (float(nclu) < float(size)/1024.0)
		nclu++;
	string FAT = readFAT();
	int check = 1;
	int vi_tri_ghi = -3;
	for (int i = 7; i < get_size_FAT(); i++) {
		if (FAT[i] == '0') {
			for (int j = 0; j < nclu; j++) {
				if (FAT[i+j] == '1')
					check = 0;
			}
			if (check == 1)
			{
				vi_tri_ghi = i;
				break;
			}
		}
		check = 1;
	}
	if (nclu == 0)
		nclu++;
	writeBlock(all, nclu, vi_tri_ghi);
	updateFAT(nclu, vi_tri_ghi, '1');
	string mo_rong = name.substr(name.length() - 3, 3);
	string nf = name.replace(name.length() - 4, 4, "");
	if (nf.length() < 16) {
		string xx="";
		for (int i = 0; i < 16 - nf.length(); i++)
			xx += ' ';
		nf = xx + nf;
	}
	TapTin add(get_size_RDET(), nf, mo_rong, size, 0, vi_tri_ghi, "");
	addFile_RDET(add);
	return true;
}
bool Outport(string name,string name_new) {
	int size = get_size_RDET();
	if (name.length()==0)
		return false;
	TapTin* x = new TapTin[size];
	x = readRDET();
	int vi_tri;
	if (CheckName(name, vi_tri))
	{
		cout << "Khong tim thay file\n";
		return false;
	}
	char* c = new char[512 * 2 * 4];
	string xx = readBlock(4, 3, c);
	int so_entry = get_size_RDET();
	so_entry--;
	TapTin out;
	out.doc(xx, 8 + vi_tri * 52);
	string result;
	int nclu = out.size / 1024;
	if (float(nclu) < float(out.size) / 512.0)
		nclu++;
	char* ccc = new char[nclu * 1024];
	result = readBlock(nclu, out.block_bd, ccc);
	result = result.substr(0, out.size);
	ofstream ofs;
	ofs.open(name_new,ios::in);
	cout << endl << result.length() << endl;
	ofs << result;
	ofs.close();
}
bool delEntry(int stt) {
	int size = get_size_RDET();
	if (stt<0 || stt>size - 1)
	{
		cout << "So thu tu khong tim thay\n";
		return false;
	}
	char* xx = new char[512 * 2 * 4];
	string s = readBlock(4, 3, xx);
	TapTin* x = new TapTin[size];
	x = readRDET();
	for (int i = stt+1; i < size; i++) {
		x[i].stt = i - 1;
		s = x[i].ghi(s, 8 + (i - 1) * 52);
	}
	string temp = TransToOptions(size-1, 10);
	s.replace(0 + 8 - temp.length(), temp.length(), temp);
	writeBlock(s, 4, 3);
	int nclu = x[stt].size / 1024;
	if (float(nclu) < float(size) / 1024.0)
		nclu++;
	updateFAT(nclu,x[stt].block_bd,'0');
	return true;
}
bool setPWFile(int stt) {
	int size = get_size_RDET();
	if (stt<0 || stt>size - 1)
	{
		cout << "So thu tu khong tim thay\n";
		return false;
	}
	char* xx = new char[512 * 2 * 4];
	string s = readBlock(4, 3, xx);
	TapTin* x = new TapTin[size];
	x = readRDET();
	string pw = "";
	string pw1 = "";
	string old_pw = s.substr(22, 8);
	cout << endl << x[stt].mode << endl;
	if (x[stt].mode==0)
	{
		while (pw.length() < 8 | pw != pw1) {
			cout << "Nhap mat khau 8 ky tu:";
			cin >> pw;
			cout << "Xac nhan lai mat khau:";
			cin >> pw1;
			if (pw.length() < 8)
				cout << "Mat khau khong dung kich thuoc\n";
			if (pw != pw1) {
				cout << "Mat khau khong dong nhat\n";
			}
		}
		x[stt].mode = 1;
		s.replace(8+stt*52+44, 8, pw);
		s.replace(8 + stt * 52 + 35, 1, "1");
		cout << s.length() << endl;
		writeBlock(s, 4, 3);
		return 1;
	}
	else
	{
		while (1) {
			cout << "Nhap mat khau cu:";
			cin >> pw;
			if (pw == old_pw) {
				while (pw.length() < 8 | pw != pw1) {
					cout << "Nhap mat khau 8 ky tu:";
					cin >> pw;
					cout << "Xac nhan lai mat khau:";
					cin >> pw1;
					if (pw.length() < 8)
						cout << "Mat khau khong dung kich thuoc\n";
					if (pw != pw1) {
						cout << "Mat khau khong dong nhat\n";
					}
				}
			}
			else {
				cout << "Mat khau cu khong trung khop\n";
			}
			s.replace(8 + stt * 52 + 44, 8, pw);
			writeBlock(s, 4, 3);
		}
	}
}
