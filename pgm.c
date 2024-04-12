#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "pgm.h"

/* PGM dosyasinin baslik bilgilerini ekrana yazdiran fonksiyon */
void pgm_print_header(PGMInfo pgm_info) {
    printf("This is a %s type PGM image containing %d x %d pixels\n",
           pgm_info.signature,
           pgm_info.width,
           pgm_info.height);
}

/* Parametre olarak aldigi filename karakter dizisi ile temsil edilen
 * resim dosyasini fopen() ile okuma modunda acar. PGM dosyasinin 
 * basligini ve piksellerini PGMInfo turunden bir struct'a atar ve 
 * donus degeri olarak bu struct'i dondurur */
PGMInfo pgm_read(const char *filename) {
    
    /* PGMInfo turunden bir degisken yaratip, error alanini 0 yapilir */
    PGMInfo pgm_info = {.error = 0};

    /* Dosyadaki satirlari okumak icin gerekli tampon olusturulur */
    char line[LINE_MAX];
    int i = 0;
    int read = 0;

    /* Dosya acilir. Eger dosya acilamazsa pgm_info'nun error
     * uyesini PGM_ERROR_READ yapip geri doner */
    FILE *pgm = fopen(filename, "rb");
    if (pgm == NULL) {
        pgm_info.error = PGM_ERROR_READ;
        return pgm_info;
    }

    /* Dosyadan tam bir satir line tamponuna okunur */
    fgets(line, sizeof(line), pgm);
    /* Daha sonra sscanf() ile bu tampondan "%s " ile okuma yapilir */
    sscanf(line, "%s ", pgm_info.signature);

    /* PGM imzasi P2 veya P5 degilse dosyayi kapatir, error'u
     * PGM_ERROR_SIGNATURE yapip geri doner */
    if (pgm_info.signature[1] != '2' && pgm_info.signature[1] != '5') {
        fclose(pgm);
        pgm_info.error = PGM_ERROR_SIGNATURE;
        return pgm_info;
    }

    /* Comment satirini okur */
    //fgets(pgm_info.comment, sizeof(pgm_info.comment), pgm);

    /* En ve boy bilgisini okur */
    fgets(line, sizeof(line), pgm);
    sscanf(line, "%d %d ", &pgm_info.width, &pgm_info.height);

    /* Max piksel degerini okur */
    fgets(line, sizeof(line), pgm);
    sscanf(line, "%u ", &pgm_info.max_pixel_value);

    /* pgm_info.pixels icin malloc() ile yer ayirilir
     * Bir piksel 1 byte yer istiyor, unutmayalim */
    pgm_info.pixels = malloc(pgm_info.width * pgm_info.height);

    /* malloc() ile yer ayrilamazsa dosya kapatilir, error'u
     * PGM_ERROR_MALLOC yapip geri doner */
    if (pgm_info.pixels == NULL) {
        fclose(pgm);
        pgm_info.error = PGM_ERROR_MALLOC;
        return pgm_info;
    }

    /* 2 farkli dosya bicimi, 2 farkli okuma bicimi var
     * P2 yani ASCII olanda dosya fgets() ile satir satir okunur
     * Okunan satirin sonundaki '\n' karakteri kaldirilir
     * ve elde kalan string atoi() ile sayiya cevirilip pixels
     * dizisine kaydedilir
     *
     * P5 yani binary olanda ise dogrudan fread() ile bir kerede pixels
	 * dizisine okuma yapilir */
	 
    switch (pgm_info.signature[1]) {
        case '2':
            /* ASCII PGM */
            while (fgets(line, LINE_MAX, pgm)) {
                line[strlen(line) - 1] = '\0';
                pgm_info.pixels[read] = (unsigned char) atoi(line);
                read += 1;
            }
            break;
        case '5':
            /* Binary PGM */
            read = fread(pgm_info.pixels, 1, pgm_info.width * pgm_info.height, pgm);
            break;
    }
    fclose(pgm);

    /* Eger dogru okuma yapamadiysaniz program assert() sayesinde
     * yarida kesilir */
    printf("Read %d bytes. (Should be: %d)\n", read, pgm_info.width * pgm_info.height);
    assert(read == (pgm_info.width * pgm_info.height));

    return pgm_info;
}

/* Parametre olarak aldigi filename karakter dizisi ile temsil edilen
 * resim dosyasini yazma modunda acarak pgm_info'nun icindeki baslik 
 * ve piksel bilgilerini dosyaya yazar. Boylece yeni bir PGM resim 
 * dosyasi yaratilmis ve kaydedilmis olur. 
 * Fonksiyon herhangi bir hatada 1, basari durumunda 0 dondurur */
int pgm_write(const char *filename, PGMInfo pgm_info) {
    
    /* Dosya okuma icin acilir. Acma basarisiz olursa fonksiyon
     * 1 dondurerek sonlanir */
    FILE *pgm = fopen(filename, "wb");
    int i;
    if (pgm == NULL) {
        return 1;
    }

    /* Baslik yapisi fprintf() ile dosyaya yazilir */
    fprintf(pgm, "%s\n%s%d %d\n%u\n", pgm_info.signature,
                                      pgm_info.comment,
                                      pgm_info.width, pgm_info.height,
                                      pgm_info.max_pixel_value);

    /* 2 farkli dosya bicimi, 2 farkli yazma bicimi */
    switch (pgm_info.signature[1]) {
        case '2':
            /* ASCII PGM */
            for (i = 0; i < pgm_info.width * pgm_info.height; ++i) {
                fprintf(pgm, "%d\n", pgm_info.pixels[i]);
            }
            break;
        case '5':
            /* Binary PGM */
            fwrite(pgm_info.pixels, 1, pgm_info.width * pgm_info.height, pgm);
            break;

    }
    /* Dosya kapatilir */
    fclose(pgm);
    return 0;
}
