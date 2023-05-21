#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/soundcard.h>
#define LENGTH 10
#define RATE 8000
#define SIZE 8
#define CHANNELS 1
unsigned char buf[(LENGTH*RATE*SIZE*CHANNELS)/8];

int main()
{
	int fd;
	int arg = 0;
	int status;
	int check, baits;
	fd = open("/dev/dsp", O_RDWR); // подготовка к записи звука
	if (fd < 0) { 
		perror("open of /dev/dsp failed");
		exit(1);
	}
	
	status = ioctl(fd, MIXER_READ(SOUND_MIXER_VOLUME), &arg);
	if (status == -1)
		perror("Error\n");
	printf("VALUE is %d\n",arg);
	printf("0	Volume		: %s available\n",arg & 1 ? "" : "not");
	printf("0	Bass		: %s available\n",arg & 2 ? "" : "not");
	printf("0	Trebl		: %s available\n",arg & 4 ? "" : "not");
	printf("0	Synth		: %s available\n",arg & 8 ? "" : "not");
	printf("1	Pcm			: %s available\n",arg & 16 ? "" : "not");
	printf("0	Spkr		: %s available\n",arg & 32 ? "" : "not");
	printf("0	Line		: %s available\n",arg & 64 ? "" : "not");
	printf("0	Mic			: %s available\n",arg & 128 ? "" : "not");
	printf("0	CD			: %s available\n",arg & 256 ? "" : "not");
	printf("0	Mix 		: %s available\n",arg & 512 ? "" : "not");
	printf("0	Pcm2		: %s available\n",arg & 1024 ? "" : "not");
	printf("0	Rec 		: %s available\n",arg & 2048 ? "" : "not");
	printf("1	IGain		: %s available\n",arg & 4096 ? "" : "not");
	printf("0	OGain		: %s available\n",arg & 8192 ? "" : "not");
// 	close(fd);
	/* В процессе выполнения лабораторной работы было обнаружено,
	 * что по информации содержащейся в прикладываемых методических указаниях
	 * не является возможным выполнить задание №5 полностью. Сделаю предположение что
	 * это может быть связано с нехваткой информации.
	 * P.S. 1 - параметры для которых удалось получить данные
	 * 0 - не удалось
	*/
	
	
	
	
	
// 	return 0;


	arg = SIZE;
	// установка размера записи
	status = ioctl(fd, SOUND_PCM_WRITE_BITS, &arg);
	if (status == -1)
		perror("SOUND_PCM_WRITE_BITS ioctl failed");
	if (arg != SIZE)
		perror("unable to set sample size");
	
	arg = CHANNELS;
	// установка каналов для записи
	status = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg);
	if (status == -1)
		perror("SOUND_PCM_WRITE_CHANNELS ioctl failed");
	if (arg != CHANNELS)
		perror("unable to set number of channels");
	
	arg = RATE;
	// установка частоты дискретизации
	status = ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);
	if (status == -1)
		perror("SOUND_PCM_WRITE_WRITE ioctl failed");
	
	while (1) { 
		printf("Say something:\n");
		baits = 0;
		check = sizeof(buf);
		while(check > 0)//Запись звука
		{
			status = read(fd, buf + baits, sizeof(buf));
			check -= status;
			baits += status;
		}
		
		char h;
		int i;
		int size = sizeof(buf);
		
		// поворот с переподвыподвертом
		for(i = 0; i <= size; i++)
		{
			h = buf[i];
			buf[i] = buf[size];
			buf[size] = h;
			size--;
		}
		
		printf("You said:\n");
		status = write(fd, buf, sizeof(buf)); // проигрывание звука
		if (status != sizeof(buf))
			perror("wrote wrong number of bytes");

		// ожидания завершения проигрывания записи
		status = ioctl(fd, SOUND_PCM_SYNC, 0);
		if (status == -1)
			perror("SOUND_PCM_SYNC ioctl failed");
			
		int choice = 0;
		printf("Продолжить? ( 1 - Да 0 - Нет )\n --> ");
		scanf("%d",&choice);
		if (!choice)
			break;
	}	
}
