#include "header.h"

int main(int argc, char *argv[]) {
    ListParent L;
    addressParent salesDicari;
    addressChild propDicari; // properti
    DataParent dataSales;
    DataChild dataProperti, dataPropBaru;
    int menuUtama, pilihanSales, pilihProperti, pilihLaporan;
    string nipBaru, nip;
    string noProp;

    inisialisasi(&L);

    do {
        showMenu();
        scanf("%d", &menuUtama);

        switch (menuUtama) {
            case 1: // MENU SALES (parent)
                do {
                    menuSales();scanf("%d", &pilihanSales);
                    getchar();
                    switch (pilihanSales) {
                        case 1: // Tambah Sales
                            printf("\n\t--- TAMBAH SALES ---\n");

                            printf("\tNIP           : ");fflush(stdin);gets(dataSales.NIP);
                            if (strcmp(dataSales.NIP, "") == 0){
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
							if(isEmpty(L)){
                        		err("Data Sales Masih Kosong");
                        	    break;
							}
                            else{
								printf("\n\t--- UBAH SALES ---\n");

	                            printf("\tNIP Sales yang diubah : ");fflush(stdin);gets(nip);
	
	                            salesDicari = findParent(L, nip);
	                            if (salesDicari == NULL) {
	                                err("Data Sales Tidak Ditemukan");
	                                break;
	                            }
	
	                            printf("\tNIP Baru (kosongkan jika tetap) : ");fflush(stdin);gets(nipBaru);
	                            if (strcmp(nipBaru, "") == 0)
	                                strcpy(nipBaru, nip);
	                            strcpy(dataSales.NIP, nipBaru);
	
	                            printf("\tNama Baru          : ");fflush(stdin);gets(dataSales.nama);
								printf("\tNomor Telepon Baru : ");fflush(stdin);gets(dataSales.nomorTelpon);
	
	                            updateParent(&L, nip, dataSales);
							}
                            break;

                        case 3: // Hapus Sales
                            printf("\n\t--- HAPUS SALES ---\n");

                            printf("\tNIP Sales yang dihapus : ");fflush(stdin);gets(nip);
                            printf("\tNIP Sales pengganti : ");fflush(stdin);gets(nipBaru); // properti berpindah ke sales yang dipilih
                            
                            // nunggu prosedur insertChildnya
                            
                            deleteAtParent(&L, nip);
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
                	menuProperti();scanf("%d", &pilihProperti);
                    getchar();
                	switch(pilihProperti){
                		case 1:{
                			printf("\n\t--- TAMBAH PROPERTI ---\n");
                            if(isEmpty(L)){
                        		err("Data Sales Masih Kosong");
                        	    break;
							}
                            printf("Masukkan NIP Sales  : ");fflush(stdin);gets(nip);
                            salesDicari = findParent(L, nip);

                            printf("Masukkan nomor properti : ");fflush(stdin);gets(dataProperti.nomorProperti);
                            if(strcmp(dataProperti.nomorProperti, "") == 0){
                                err("Nomor properti tidak boleh kosong");
                                break;
                            }
                            propDicari = findChild(salesDicari->firstChild, dataProperti.nomorProperti);
                            if(propDicari != NULL){
                                err("Nomor Properti Sudah Ada");
                                break;
                            }

                            printf("Masukkan jenis properti (rumah/tanah)   : ");fflush(stdin);gets(dataProperti.jenis);
                            if(strcmp(dataProperti.jenis, "rumah") != 0 && strcmp(dataProperti.jenis, "tanah") != 0){
                                err("Jenis Harus Rumah atau Tanah");
                                break;
                            }

                            printf("Masukkan alamat properti     : ");fflush(stdin);gets(dataProperti.alamat);
                            printf("Masukkan harga properti : ");scanf("%d", &dataProperti.harga);
                            insertLastChild(&L, nip, dataProperti);
							break;
						}

                        case 2:{
                            printf("\n\t--- UBAH PROPERTI ---\n");
                            if(isEmpty(L)){
                        		err("Data Sales Masih Kosong");
                        	    break;
							}
                            else{
                                printf("Masukkan NIP Sales  : ");fflush(stdin);gets(nip);
                                printf("Masukkan nomor properti : ");fflush(stdin);gets(noProp);
                                printf("Masukkan jenis properti (tanah/rumah)   : ");fflush(stdin);gets(dataPropBaru.jenis);
                                if(strcmp(dataPropBaru.jenis, "rumah") != 0 && strcmp(dataPropBaru.jenis, "tanah") != 0){
                                    err("Jenis Harus Rumah atau Tanah");
                                    break;
                                }

                                printf("Alamat properti baru : ");fflush(stdin);gets(dataPropBaru.alamat);
                                printf("Harga properti baru : ");scanf("%d", &dataPropBaru.harga);
                                updateChild(&L, nip, noProp, dataPropBaru);
                            }
                            break;
                        }

                        case 3:{
                            printf("\n\t--- HAPUS PROPERTI ---\n");
                            if(isEmpty(L)){
                        		err("Data Sales Masih Kosong");
                        	    break;
							}
                            else{
                                printf("Masukkan NIP Sales  : ");fflush(stdin);gets(nip);
                                printf("Masukkan nomor properti : ");fflush(stdin);gets(noProp);
                                deleteAtChild(&L, nip, noProp);
                            }
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

            case 3: // JUAL PROPERTI (belum dibuat)
                err("Fitur Jual Properti Belum Tersedia");
                getch();
                break;

            case 4: // LAPORAN OWNER (belum dibuat)
                do{
                	menuLaporan();scanf("%d", &pilihLaporan);
                	switch(pilihLaporan){
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
