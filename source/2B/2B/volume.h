#pragma once
#include<iostream>
#include<fstream>
using namespace std;
#include<stdio.h>
#include<string>
#include"ChuyenCoSo.h"
string readSector(int nsect, int lsect, char* x);// tra ve 2 cho la string va char*
string readBlock(int nblock, int lblock, char* x);// tra ve 2 cho la string va char*
int createVolume();// tao volume tra ve kich thuoc volume
void writeSector(string buf, int nsect, int lsect);
void writeBlock(string buf, int nblock, int lblock);
void formatVolume(int size_volume, string password = "");
string readFAT();//chiem 2 cluster
void updateFAT(int nblock, int lblock, char giaTri);
int get_size_FAT();
string readAll();
class TapTin {
public:
	int stt;//8B
	string TenTapTin;//16B
	string PhanMoRong;//3B
	int size;//8B
	int mode;//1B
	int block_bd;//8B
	string password;//8B
	//1 entry 52B
public:
	void xuat();
	void doc(string s, int vi_tri_bd);
	string ghi(string s, int vi_tri_bd);//thay thế trên RDET
	void nhap();
	TapTin(){
	}
	TapTin(int, string, string, int, int, int, string);
};
