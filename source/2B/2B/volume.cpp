#include"volume.h"
string readSector(int nsect, int lsect, char* x) {
	ifstream out;
	out.open("MYFS.dat", ios::out);
	out.seekg(lsect * 512);
	out.read(x, nsect * 512);
	out.close();
	string s;
	for (int i = 0; i < nsect * 512;i++) {
		s += x[i];
	}
	return s;
}
string readBlock(int nblock, int lblock, char* x) {
	string s = readSector(nblock * 2, lblock * 2, x);
	return s;
}
string readAll() {
	string s;
	string temp;
	ifstream out;
	out.open("MYFS.dat");
	while (!out.eof()) {
		getline(out, temp, '\n');
		s += temp;
		if (!out.eof())
			s += "\n";
	}
	out.close();
	return s;
}
int createVolume() {
	string name = "MYFS.dat";
	int size;
	do {
		cout << "Nhap kich thuoc theo Mb (>=1&&<=4000):"; // kích thước tương ứng FAT16
		cin >> size;
	} while (size < 1 || size>5000);
	ofstream in;
	in.open(name ,ios::binary);
	char x[1024];
	for (int i = 0; i < 1024; i++) {
		x[i] = '0';
	}
	for (int i = 0; i < size*1024; i++) {
		in.write(x, 1024);
	}
	in.close();
	return size;
}
void writeSector(string buf, int nsect, int lsect) {
	if (buf.length() > nsect * 512) {
		cout << "/nErorr. Size of string is higher than size of n sector\n";
		return;
	}
	ifstream out;
	out.open("MYFS.dat", ios::out);
	string all = readAll();
	all.replace(lsect * 512, buf.length(), buf);
	ofstream in;
	in.open("MYFS.dat", ios::binary);
	in << all;
	in.close();
}
void writeBlock(string buf,int nblock, int lblock) {
	writeSector(buf, nblock * 2, lblock * 2);
}
void formatVolume(int size_volume, string password) {
	//kích thước sector(4B), số sector(8B), 
	//số sector của cluster(1B), số cluster(8B), mode(1B), mật khẩu nếu có(8B)
	string temp = "";
	string size_sector = "0512";
	string num_sec = TransToOptions(size_volume / 512, 10);
	temp = "00000000";
	num_sec = temp.replace(temp.length() - num_sec.length(), num_sec.length(), num_sec);
	num_sec = temp;
	string num_sec_cluster = "2";
	string num_clu = TransToOptions(size_volume / 1024, 10);
	temp = "00000000";
	num_clu = temp.replace(temp.length() - num_clu.length(), num_clu.length(), num_clu);
	string mode;
	if (password.length() == 0) {
		mode = "0";
		password = "00000000";
	}
	else {
		mode = "1";
	}
	string sector0 = size_sector + num_sec + num_sec_cluster + num_clu + mode + password;
	writeSector(sector0, 1, 0);
	updateFAT(7, 0, '1');
}
// bang FAT chiem 2 cluster gia tri tung phan tu la 0|1
string readFAT() {
	char* x = new char[2048];
	return readBlock(2, 1, x);
}
void updateFAT(int nblock, int lblock, char giaTri) {
	char* x = new char[2048];
	string s = readBlock(2, 1, x);
	for (int i = 0; i < nblock; i++) {
		s[lblock+i] = giaTri;
	}
	writeBlock(s, 2, 1);
}
int get_size_FAT() {
	char* x = new char[512];
	string s  = readSector(1, 0, x);
	return StranToDe(s.substr(13, 8), 10);
}
void TapTin::xuat() {
	cout << stt << "\t" << TenTapTin;
	if (PhanMoRong.length() != 0)
		cout << '.' << PhanMoRong;
	cout << "\t" << size;
	cout << "\t" << mode << "\t" << block_bd << "\n";
}
void TapTin::doc(string s, int vi_tri_bd)
{
	stt = StranToDe(s.substr(vi_tri_bd, 8), 10);
	vi_tri_bd += 8;
	TenTapTin = s.substr(vi_tri_bd, 16);
	vi_tri_bd += 16;
	PhanMoRong = s.substr(vi_tri_bd, 3);
	vi_tri_bd += 3;
	size = StranToDe(s.substr(vi_tri_bd, 8), 10);
	vi_tri_bd += 8;
	if (s.substr(vi_tri_bd, 1)[0] == '0')
		mode = 0;
	else
		mode = 1;
	vi_tri_bd += 1;
	block_bd = StranToDe(s.substr(vi_tri_bd, 8), 10);
	vi_tri_bd += 8;
	password = s.substr(vi_tri_bd, 8);
}
string TapTin::ghi(string s, int vi_tri_bd) {
	string temp = TransToOptions(stt, 10);
	s.replace(vi_tri_bd + 8 - temp.length(), temp.length(), temp);
	vi_tri_bd += 8;
	s.replace(vi_tri_bd + 16 - TenTapTin.length(), TenTapTin.length(), TenTapTin);
	vi_tri_bd += 16;
	s.replace(vi_tri_bd + 3 - PhanMoRong.length(), PhanMoRong.length(), PhanMoRong);
	vi_tri_bd += 3;
	temp = TransToOptions(size, 10);
	s.replace(vi_tri_bd + 8 - temp.length(), temp.length(), temp);
	vi_tri_bd += 8;
	if (mode == 0)
		s.replace(vi_tri_bd, 1, "0");
	else
		s.replace(vi_tri_bd, 1, "1");
	vi_tri_bd += 1;
	temp = TransToOptions(block_bd, 10);
	s.replace(vi_tri_bd + 8 - temp.length(), temp.length(), temp);
	vi_tri_bd += 8;
	s.replace(vi_tri_bd, 8, password);
	return s;
}
void TapTin::nhap()
{
	cout << "Nhap STT:";
	cin >> stt;
	cout << "Nhap ten tap tin:";
	cin >> TenTapTin;
	cout << "Nhap phan mo rong:";
	cin >> PhanMoRong;
	cout << "Nhap size:";
	cin >> size;
	cout << "Nhap mode 0|1:";
	cin >> mode;
	cout << "Nhap block bat dau:";
	cin >> block_bd;
	cout << "Nhap mat khau:";
	cin >> password;
}
TapTin::TapTin(int STT, string ten, string mr, int s, int m, int bl_bd, string pw) {
	stt = STT;
	TenTapTin = ten;
	PhanMoRong = mr;
	size = s;
	mode = m;
	block_bd = bl_bd;
	if (mode == '0')
		password = "00000000";
	else password = pw;
}
/*
int main()
{
	formatVolume(1024 * 1024, "03562278");
	char* x = new char[1024];
	readBlock(1, 0, x);
	for (int i = 0; i < 1024; i++)
		cout << x[i];
	return 0;
}*/