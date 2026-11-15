#include "header.h"

int main(int argc, char *argv[]) {
    ListParent L;
    addressParent salesDicari;
    DataParent dataSales;
    int menuUtama, pilihanSales, pilihProperti, pilihLaporan;
    string nipLama, nipBaru, nipPengganti;

    inisialisasi(&L);

    do {
        showMenu();
        scanf("%d", &menuUtama);

        switch (menuUtama) {
            case 1: // MENU SALES (parent)
                do {
                    menuSales();scanf("%d", &pilihanSales);
                    switch (pilihanSales) {
                        case 1: // Tambah Sales
                            printf("\n\t--- TAMBAH SALES ---\n");

                            printf("\tNIP           : ");fflush(stdin);gets(dataSales.NIP);
                            if (strcmp(dataSales.NIP, "") == 0) {
                                err("NIP Tidak Boleh Kosong");
                                break;
                            }

                            printf("\tNama          : ");fflush(stdin);gets(dataSales.nama);
							printf("\tNomor Telepon : ");fflush(stdin);gets(dataSales.nomorTelpon);

                            // Sales baru selalu mulai dari 0, bertambah saat properti terjual
                            dataSales.totalPenjualan = 0;

                            insertLastParent(&L, dataSales);
                            break;

                        case 2: // Ubah Sales
                            printf("\n\t--- UBAH SALES ---\n");

                            printf("\tNIP Sales yang diubah : ");fflush(stdin);gets(nipLama);

                            salesDicari = findParent(L, nipLama);
                            if (salesDicari == NULL) {
                                err("Data Sales Tidak Ditemukan");
                                break;
                            }

                            printf("\tNIP Baru (kosongkan jika tetap) : ");fflush(stdin);gets(nipBaru);
                            if (strcmp(nipBaru, "") == 0)
                                strcpy(nipBaru, nipLama);
                            strcpy(dataSales.NIP, nipBaru);

                            printf("\tNama Baru          : ");fflush(stdin);gets(dataSales.nama);
							printf("\tNomor Telepon Baru : ");fflush(stdin);gets(dataSales.nomorTelpon);

                            updateParent(&L, nipLama, dataSales);
                            break;

                        case 3: // Hapus Sales
                            printf("\n\t--- HAPUS SALES ---\n");

                            printf("\tNIP Sales yang dihapus : ");fflush(stdin);gets(nipLama);
                            printf("\tNIP Sales pengganti : ");fflush(stdin);gets(nipPengganti); // properti berpindah ke sales yang dipilih
                            
                            // nunggu prosedur insertChildnya
                            
                            deleteAtParent(&L, nipLama);
                            break;

                        case 4: // Lihat Semua Sales
                            printf("\n\t--- DAFTAR SALES ---");
                            printAllParent(L);
                            break;

                        case 0: // Kembali ke menu utama
                            break;

                        default:
                            err("Pilihan Tidak Tersedia");
                            break;
                    }
                    getch();
                } while (pilihanSales != 0);
                break;

            case 2: // MENU PROPERTI (child, belum dibuat)
                do{
                	menuProperti();scanf("%d", pilihProperti);
                	switch(pilihProperti){
                		case 1:{
                			
							break;
						}
						
						case 0:{
							break;
						}
						
						default:{
							err("Pilihan Tidak Tersedia");
							break;
						}
					}
                	getch();
				}while(pilihProperti != 0);
                break;
                break;

            case 3: // JUAL PROPERTI (belum dibuat)
                err("Fitur Jual Properti Belum Tersedia");
                getch();
                break;

            case 4: // LAPORAN OWNER (belum dibuat)
                do{
                	menuLaporan();scanf("%d", &pilihLaporan);
                	switch(menu){
                		case 1:{	
						break;
						}
						
						case 0:{
							break;
						}
						
						default:{
							err("Pilihan Tidak Tersedia");
							break;
						}
					}
				}while(pilihLaporan!=0);
                break;

            case 0: // Keluar program
                break;

            default:
                err("Menu Tidak Tersedia");
                getch();
                break;
        }
    } while (menuUtama != 0);

    system("cls");
    suc("Terima Kasih Telah Menggunakan Program Ini");

    return 0;
}
