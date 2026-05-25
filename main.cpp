#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <stdlib.h>
struct PE_CONTEXT {
	LPVOID pFileBuffer{ nullptr };
	SIZE_T FileSize{};
	//DOS头和NT头的指针
	PIMAGE_DOS_HEADER pDos{ nullptr };
	PIMAGE_NT_HEADERS pNT{ nullptr };
	//标志位，表示PE文件是否有效
	BOOL isVaild{ FALSE };
};
//将修改后的PE文件写入磁盘
BOOL WriteFile(PE_CONTEXT pe,char* str);
//加载文件到内存
PE_CONTEXT LoadFileToMemory(IN LPCSTR str);






int main(int argc,char** argv) {
	std::setlocale(LC_ALL, ".UTF-8");
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " <PE_FILE>" << std::endl;
		return 1;
	}
	PE_CONTEXT pe = LoadFileToMemory(argv[1]);
	if (!pe.pFileBuffer) {
		std::cout << "[-]加载文件失败" << std::endl;
		return 1;
	}
	pe.pDos = (PIMAGE_DOS_HEADER)pe.pFileBuffer;
	pe.pNT = (PIMAGE_NT_HEADERS)((DWORD_PTR)pe.pFileBuffer + pe.pDos->e_lfanew);
	//修改ASLR
	pe.pNT->OptionalHeader.DllCharacteristics &= ~IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE;
	//输出ASLR状态
	printf("[-] ASLR Status: %s\n", (pe.pNT->OptionalHeader.DllCharacteristics & IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE) ? "Enabled" : "Disabled");
	if (WriteFile(pe, argv[1])) {
		std::cout << "[+]文件写入成功" << std::endl;
	}
	return 0;
}



//加载文件到内存
PE_CONTEXT LoadFileToMemory(IN LPCSTR str) {
	PE_CONTEXT pe{};
	//读取文件
	FILE* File{ fopen(str, "rb") };
	if (!File) {
		return pe;
	}
	//把指针放在末尾,获取文件大小
	fseek(File, 0, SEEK_END);
	pe.FileSize = (SIZE_T)ftell(File);
	fseek(File, 0, SEEK_SET);

	//分配内存
	pe.pFileBuffer = malloc(pe.FileSize);
	if (!pe.pFileBuffer) {
		std::cout << "[-]分配内存失败" << std::endl;
		fclose(File);
		return pe;
	}
	//读取文件到内存
	size_t ReadSize = fread(pe.pFileBuffer, 1, pe.FileSize, File);
	if (ReadSize!=pe.FileSize) {
		std::cout << "[-]读取文件失败" << std::endl;
		free(pe.pFileBuffer);
		fclose(File);
		return pe;
	}
	fclose(File);
	pe.isVaild = TRUE;
	return pe;
}







//将修改后的PE文件写入磁盘
BOOL WriteFile(PE_CONTEXT pe, char* str) {
	FILE* hFile = fopen("New.exe", "wb");
	if (!hFile) {
		std::cout << "写入期间打开文件失败" << std::endl;
		return FALSE;
	}
	fwrite(pe.pFileBuffer, 1, pe.FileSize, hFile);
	fclose(hFile);
	return TRUE;
}
