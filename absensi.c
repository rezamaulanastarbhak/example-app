#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definisi nama file agar mudah diubah jika perlu
#define FILENAME "absensi.txt"

// Deklarasi fungsi agar bisa dipanggil dari mana saja
void mainMenu();
void checkAndCreateHeader();
void tambahData();
void tampilkanData();
void updateData();
void hapusData();

// Fungsi untuk membersihkan buffer input (penting setelah scanf)
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fungsi utama yang menjalankan program
int main() {
    int pilihan;
    checkAndCreateHeader(); // Pastikan file dan header ada saat program pertama kali jalan

    while (1) {
        mainMenu();
        if (scanf("%d", &pilihan) != 1) {
            printf("Input tidak valid, mohon masukkan angka.\n");
            clearInputBuffer(); // Bersihkan input yang salah
            getchar(); // Tunggu user menekan Enter
            continue;
        }
        clearInputBuffer(); // Bersihkan newline setelah scanf

        switch (pilihan) {
            case 1:
                tambahData();
                break;
            case 2:
                tampilkanData();
                break;
            case 3:
                updateData();
                break;
            case 4:
                hapusData();
                break;
            case 5:
                printf("Terima kasih telah menggunakan program ini!\n");
                exit(0); // Keluar dari program
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                break;
        }
        printf("\nTekan Enter untuk kembali ke menu...");
        getchar(); // Pause
    }
    return 0;
}

// Fungsi untuk menampilkan menu utama
void mainMenu() {
    system("cls || clear"); // Membersihkan layar (untuk Windows atau Linux/Mac)
    printf("========================================\n");
    printf("   Program Manajemen Absensi Siswa\n");
    printf("========================================\n");
    printf("1. Tambah Data Absensi\n");
    printf("2. Tampilkan Semua Data\n");
    printf("3. Update Data Siswa\n");
    printf("4. Hapus Data Siswa\n");
    printf("5. Keluar\n");
    printf("----------------------------------------\n");
    printf("Masukkan pilihan Anda: ");
}

// Fungsi untuk membuat file dan header jika belum ada
void checkAndCreateHeader() {
    FILE *fptr = fopen(FILENAME, "r");
    if (fptr == NULL) { // Jika file tidak ada
        fptr = fopen(FILENAME, "w"); // Buat file baru
        fprintf(fptr, "+----+--------------------------------+----------------+\n");
        fprintf(fptr, "| No | Nama Siswa                     | Keterangan     |\n");
        fprintf(fptr, "+----+--------------------------------+----------------+\n");
        fclose(fptr);
    } else {
        fclose(fptr); // Jika file sudah ada, tutup saja
    }
}


// Fungsi untuk menambah data baru
void tambahData() {
    char nama[50];
    char keterangan[15];
    int pilihan_ket;
    int no = 0;
    
    FILE *fptr;

    // Hitung nomor terakhir dari file yang ada
    fptr = fopen(FILENAME, "r");
    char buffer[100];
    while(fgets(buffer, sizeof(buffer), fptr) != NULL) {
        if (sscanf(buffer, "| %d", &no) == 1) {
            // berhasil membaca nomor
        }
    }
    fclose(fptr);
    no++; // Tambah 1 untuk nomor baru

    // Buka file dalam mode append (menambah di akhir)
    fptr = fopen(FILENAME, "a");
    if (fptr == NULL) {
        printf("Gagal membuka file!\n");
        return;
    }

    printf("\n--- Menambah Data Baru ---\n");
    printf("Masukkan Nama Siswa: ");
    fgets(nama, 50, stdin);
    nama[strcspn(nama, "\n")] = 0; // Hapus newline dari fgets

    printf("Pilih Keterangan:\n");
    printf("1. Hadir\n2. Sakit\n3. Izin\n");
    printf("Pilihan (1-3): ");
    scanf("%d", &pilihan_ket);
    clearInputBuffer();

    switch (pilihan_ket) {
        case 1:
            strcpy(keterangan, "Hadir");
            break;
        case 2:
            strcpy(keterangan, "Sakit");
            break;
        case 3:
            strcpy(keterangan, "Izin");
            break;
        default:
            strcpy(keterangan, "N/A");
            break;
    }

    fprintf(fptr, "| %-2d | %-30s | %-14s |\n", no, nama, keterangan);
    fclose(fptr);

    printf("\n✅ Data berhasil ditambahkan!\n");
}


// Fungsi untuk menampilkan seluruh data dari file
void tampilkanData() {
    FILE *fptr = fopen(FILENAME, "r");
    char c;

    system("cls || clear");
    printf("--- Data Absensi Siswa ---\n\n");
    if (fptr == NULL) {
        printf("Gagal membuka file atau file tidak ditemukan.\n");
        return;
    }

    while ((c = fgetc(fptr)) != EOF) {
        putchar(c);
    }
    
    // Tambahkan footer secara manual saat menampilkan
    printf("+----+--------------------------------+----------------+\n");

    fclose(fptr);
}

// Fungsi untuk memperbarui data yang sudah ada
void updateData() {
    int no_update, no_baca, pilihan_ket;
    char nama[50], keterangan[15], nama_baru[50], keterangan_baru[15];
    FILE *fptr, *temp_fptr;

    tampilkanData(); // Tampilkan data agar user tahu nomor mana yang akan diubah
    printf("\nMasukkan nomor data yang ingin di-update: ");
    scanf("%d", &no_update);
    clearInputBuffer();

    fptr = fopen(FILENAME, "r");
    temp_fptr = fopen("temp.txt", "w");

    if (fptr == NULL || temp_fptr == NULL) {
        printf("Gagal memproses file.\n");
        return;
    }
    
    // Salin header ke file sementara
    char header1[100], header2[100], header3[100];
    fgets(header1, sizeof(header1), fptr);
    fgets(header2, sizeof(header2), fptr);
    fgets(header3, sizeof(header3), fptr);
    fprintf(temp_fptr, "%s%s%s", header1, header2, header3);
    
    // Baca per baris data
    int data_ditemukan = 0;
    while (fscanf(fptr, "| %d | %49[^|] | %14[^|] |\n", &no_baca, nama, keterangan) == 3) {
        if (no_baca == no_update) {
            data_ditemukan = 1;
            printf("--- Memperbarui Data No. %d ---\n", no_update);
            printf("Nama Lama: %s\n", nama);
            printf("Masukkan Nama Baru: ");
            fgets(nama_baru, 50, stdin);
            nama_baru[strcspn(nama_baru, "\n")] = 0;

            printf("Keterangan Lama: %s\n", keterangan);
            printf("Pilih Keterangan Baru (1:Hadir, 2:Sakit, 3:Izin): ");
            scanf("%d", &pilihan_ket);
            clearInputBuffer();

            switch (pilihan_ket) {
                case 1: strcpy(keterangan_baru, "Hadir"); break;
                case 2: strcpy(keterangan_baru, "Sakit"); break;
                case 3: strcpy(keterangan_baru, "Izin"); break;
                default: strcpy(keterangan_baru, "N/A"); break;
            }
            
            // Trim spasi di akhir nama dan keterangan yang dibaca dari file
            nama[strcspn(nama, " ")] = 0;
            keterangan[strcspn(keterangan, " ")] = 0;
            
            fprintf(temp_fptr, "| %-2d | %-30s | %-14s |\n", no_baca, nama_baru, keterangan_baru);
        } else {
             fprintf(temp_fptr, "| %-2d | %-30s | %-14s |\n", no_baca, nama, keterangan);
        }
    }

    fclose(fptr);
    fclose(temp_fptr);

    remove(FILENAME); // Hapus file asli
    rename("temp.txt", FILENAME); // Ganti nama file sementara menjadi nama file asli

    if (data_ditemukan) {
        printf("\n✅ Data berhasil di-update!\n");
    } else {
        printf("\n❌ Data dengan nomor %d tidak ditemukan.\n", no_update);
    }
}


// Fungsi untuk menghapus baris data
void hapusData() {
    int no_hapus, no_baca, no_baru = 1;
    char nama[50], keterangan[15];
    FILE *fptr, *temp_fptr;

    tampilkanData();
    printf("\nMasukkan nomor data yang ingin dihapus: ");
    scanf("%d", &no_hapus);
    clearInputBuffer();

    fptr = fopen(FILENAME, "r");
    temp_fptr = fopen("temp.txt", "w");

    if (fptr == NULL || temp_fptr == NULL) {
        printf("Gagal memproses file.\n");
        return;
    }

    char header1[100], header2[100], header3[100];
    fgets(header1, sizeof(header1), fptr);
    fgets(header2, sizeof(header2), fptr);
    fgets(header3, sizeof(header3), fptr);
    fprintf(temp_fptr, "%s%s%s", header1, header2, header3);

    int data_ditemukan = 0;
    while (fscanf(fptr, "| %d | %49[^|] | %14[^|] |\n", &no_baca, nama, keterangan) == 3) {
        if (no_baca == no_hapus) {
            data_ditemukan = 1; // Tandai bahwa data ditemukan dan akan dilewati (dihapus)
            continue;
        } else {
            // Tulis ulang ke file sementara dengan nomor urut yang baru
            fprintf(temp_fptr, "| %-2d | %-30s | %-14s |\n", no_baru, nama, keterangan);
            no_baru++;
        }
    }

    fclose(fptr);
    fclose(temp_fptr);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (data_ditemukan) {
        printf("\n✅ Data berhasil dihapus!\n");
    } else {
        printf("\n❌ Data dengan nomor %d tidak ditemukan.\n", no_hapus);
    }
}
