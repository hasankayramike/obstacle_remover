#include <stdio.h>
#include <stdlib.h>

#include "pgm.h"
#include "llist.h"

int main(int argc, const char *argv[])
{
    /* Her bir elemani bir fotograf olan 9 elemanli PGMInfo dizisi */
    PGMInfo images[9];
    int i, j;

    /* Uygulama ciktisi olarak uretilecek resim dosyasi icin hazirlik */
	/* Filtrelenmis resim icin PGMInfo hazirligi */
    PGMInfo filtered = { "P5", "# \n", 495, 557, 255, NULL, 0};
    filtered.pixels = malloc(filtered.width * filtered.height);

    /* Fonksiyonlarınız icin test kodu */
	/* Asagidaki dizileri kullanarak fonksiyonlarinizi test edin*/
	struct node *test = NULL, *c = NULL;
    unsigned char values[] = {10, 20, 5, 40, 2, 55, 53, 54, 1};
    unsigned char values_sorted[] = {1, 2, 5, 10, 20, 40, 53, 54, 55};
    
	/* insert fonksiyonu icin test kodu*/
	for (i = 0; i < sizeof(values); ++i) {
        test = insert(test, values[i]);
    }
    for (i = 0, c = test; i < sizeof(values); ++i) {
        if (c->data != values_sorted[i]) {
            fprintf(stderr, "insert fonksiyonu hatali!\n");
            exit(1);
        }
        c = c->next;
    }
	
	/* get_median fonksiyonu icin test kodu */
    if (get_median(test) != 20) {
        fprintf(stderr, "get_median fonksiyonu hatali!\n");
        exit(1);
    }

    printf("Fonksiyonlar duzgun calisiyor...\n");

    /* Dosyalar okunur */
    for (i = 1; i <= 9; ++i) {
        char image_file[16];
		/* okuma yapilacak dosya yoluna dikkat! */
        sprintf(image_file, "images/%d.pgm", i);
        printf("Reading %s\n", image_file);
        images[i - 1] = pgm_read(image_file);

        /* Hata olusursa programi sonlandirir */
        if (images[i - 1].error > 0) {
            pgm_print_error(images[i - 1]);
            exit(1);
        }

        /* Bilgileri ekrana yazdirir */
        pgm_print_header(images[i - 1]);
    }

    /* Bir resimdeki toplam piksel sayisi kadar donecek ana dongu */
	
    for (int i = 0; i < filtered.width * filtered.height; i++) {
        
		/* Liste icin gosterici */
        struct node *list = NULL;

        /* TODO: 9 elemanlik sirali piksel listesini olusturun */
        for (int j = 0; j < 9; j++) {
            list = insert(list, images[j].pixels[i]);
        }

        /* TODO: get_median() ile listenin ortanca degerini
         * filtered adli sonuc resminin ilgili pikseline atayin */
        unsigned char newPixel = get_median(list);
        filtered.pixels[i] = newPixel;

        /* TODO: Listeyi bosaltin */
        free_list(list);
    }

    /* TODO: Filtrelenmis resmi tutan filtered degiskenini pgm_write() 
	 * kullanarak result.pgm adli dosyaya kaydedin */
    const char* filename = "images/result.pgm";
    int new = pgm_write(filename, filtered);
	
	
    /* fotograflar icin ayrilan dizideki elemanlari salalim */
    for (i = 0; i < 9; ++i) {
        free(images[i].pixels);
    }
    free(filtered.pixels);
    
	return 0;
}
