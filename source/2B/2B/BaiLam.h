#pragma once
//cài đặt or thay đổi mật khẩu volume
int changePW();
int get_size_RDET();
//trả lại chuỗi những entry
TapTin* readRDET();
void addFile_RDET(TapTin add);
void listFile();
//kiểm tra tên file có trong bảng RDET không nếu có cật nhật vị trí
bool CheckName(string name, int& vi_tri);
bool Import(string name);
bool Outport(string name, string name_new);
//xóa tập tin tại số thứ tự
bool delEntry(int stt);
//cài đặt hoặc thay đổi mật khẩu cho tập tin
bool setPWFile(int stt);