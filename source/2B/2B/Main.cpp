#include"BaiLam.h"
#include"volume.h"
int main() {
	/*
	formatVolume(1024 * 1024, "21212121");
	changePW();
	*/
	/*
	char* x = new char[512];
	cout<<readSector(1, 0, x);
	updateFAT(10, 1, '0');
	cout << endl << readFAT();*/
	createVolume();
	formatVolume(1024 * 1024, "22334455");
	cout << "Thay doi mat khau volume\n";
	changePW();
	Import("MYFS2.txt");
	Import("MYFS3.dat");
	Import("MYFS4.txt");
	Import("MYFS5.dat");
	cout << "Bang FAT sau khi them file\n";
	cout << readFAT() << endl;
	cout << "Danh sach tap tin\n";
	listFile();
	cout << endl;
	cout << "Xoa file co stt la 2\n";
	delEntry(2);
	cout << "sau khi xoa\n";
	listFile();
	cout << "Cai password\n";
	setPWFile(2);
	cout << "Bang FAT\n";
	cout << readFAT() << endl;
	Outport("MYFS3.dat", "Outport3.dat");
	return 0;
}