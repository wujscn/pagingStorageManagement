
// *author: wujs
// *email: wujs@mail.nwpu.edu.cn

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

constexpr int PAGE = 4096; // 4k blocks
constexpr int TOTSPACE = 128 * 1024 * 1024;  // 4k
constexpr int PAGESPACE = TOTSPACE / PAGE;

int memory[PAGE];
int process[PAGE][PAGE+1]; // processes
int use_num; 
int process_num;

void check()
{
    printf("[*]Page: %d block(s), used: %d block(s), remain: %d block(s)\n", PAGE, use_num, PAGE - use_num);
    printf("[*]Used space: %d, remain: %d\n", use_num * PAGESPACE, TOTSPACE - use_num * PAGESPACE);
    printf("[*]Number of Process: %d\n", process_num);
    if (use_num < PAGE)
    {
        printf("[*]Used page blocks(%d):\n", use_num);
		int tmpcnt = 0;
		for (int i = 0; i < PAGE; i++) {
			if (memory[i] == 1) {
				if (tmpcnt % 8 == 0) {
					printf("\n");
				}
				printf("%8d", i);
				tmpcnt++;
			}
		}
    }
    if (process_num > 0) {
		printf("\n[*]Processes: \n");
		for (int i = 0; i < PAGE; i++) {
			if (process[i][0] > 0) {
				printf("    PID: %d \n    use %d (%d blocks)：", i, process[i][0] * PAGESPACE, process[i][0]);
				int tmpcnt = 0;
				for (int j = 1; j <= process[i][0]; j++)
				{
					if (tmpcnt % 8 == 0) {
						printf("\n");
					}
					printf("%8d", process[i][j]);
					tmpcnt++;
				}
                printf("\n");
			}
		}
        printf("\n");
	}
	else {
		printf("\n[*]No Process now.\n");
	}
}

void init_memory() //随机占用100块内存块
{
	memset(memory, 0, sizeof(memory)); //将所有内存块置为0，表示未使用
	const int count = 100;
	srand((unsigned)time(NULL)); //初始化随机数
	for (int i = 0; i < count; i++) {
		memory[rand() % PAGE] = 1; //产生0~99的随机数
	}
	use_num = count;  //初始化已经使用的内存 
	for (int i = 0; i < PAGE; i++) {   //初始化进程数组
		process[i][0] = 0;
		for (int j = 1; j < PAGE; j++)
			process[i][j] = -1;
	}
	process_num = 0;  //初始化进程数
	printf("Initialization:\n");
	check();
}
 
 
void create_process() //创建进程
{
	int pid, size;
	printf("[+]Please type in Pid and number of blocks it need(less than %d)\n",PAGE);
    printf("    PID>");
    std::cin >> pid;
    if (pid >= PAGE) {
		printf("out of range!\n");
		return;
	}
    printf("    Blocks it need>");
    std::cin >> size;
	if (PAGE - use_num < size) {    
		printf("No enough space!\n");
		return;
	}
	use_num += size;
	process[pid][0] = size;
	int k = 0;
	for (int i = 1;i <= size; i++) {   //检查是否存在相同进程号的进程
		while (memory[k] == 1 && k < PAGE) {
			k++;
		}
		process[pid][i] = k;
		memory[k++] = 1;
	}
	process_num++;
	printf("[-]Created.\n");
}
 
void end_process()  //结束进程
{
	int pid, size;
	if (process_num < 1) {
		printf("No process.\n");
		return;
	}
	printf("[+]Please type in PID(less than%d)", PAGE);
    printf("\nPID>");
	std::cin >> pid;
	if (process[pid][0] == 0) {
		printf("[-]Wrong PID!\n");
		return;
	}
	size = process[pid][0];
	for (int j = 1; j < size; j++)
	{
		memory[process[pid][j]] = 0;
		process[pid][j] = -1;
	}
	process[pid][0] = 0;
	process_num--;
	use_num -= size;
	printf("[-]Ended.\n");
}

void addr()
{
    printf("[+]Please type in PID and logical addr  \n");
    printf("  PID>");
    int pid;
	std::cin >> pid;
    printf("    logical addr>");
    int laddr;
    std::cin >> laddr;
    int p = laddr / PAGESPACE;
    int w = laddr % PAGESPACE;
    int b = process[pid][p] + 1;
    int maddr = b * PAGESPACE + w;
    printf("[-]Real addr is:%d \n", maddr);
}
 
void menu() //菜单
{
	printf("\n[+]Menu:\n");
	printf("    1.Create Process\n");
	printf("    2.End Process\n");
	printf("    3.Memory Usage\n");
    printf("    4.Addr convert\n");
	printf("    5.Quit\n");	
	printf("Choice>");
}

int main(int argc, char *argv[])
{
    std::cout << " __          ___      .______       ____   " << std::endl;
    std::cout << "|  |        /   \\     |   _  \\     |___ \\  " << std::endl;
    std::cout << "|  |       /  ^  \\    |  |_)  |      __) | " << std::endl;
    std::cout << "|  |      /  /_\\  \\   |   _  <      |__ <  " << std::endl;
    std::cout << "|  `----./  _____  \\  |  |_)  |     ___) | " << std::endl;
    std::cout << "|_______/__/     \\__\\ |______/     |____/  " << std::endl;
    std::cout << "                                           " << std::endl;
    
    init_memory(); /*初始化内存空间*/
    int choice;
	menu();
	while (scanf("%d", &choice) != 0) {
		switch (choice)
		{
		case 1:
			create_process();
			break;
		case 2:
			end_process();
			break;
		case 3:
			check();
			break;
        case 4:
            addr();
            break;
		case 5:
			exit(0);
		default:
			printf("Bad input. Try again!\n");
			break;
		}
		menu();
	}
	system("pause");
	return 0;
}