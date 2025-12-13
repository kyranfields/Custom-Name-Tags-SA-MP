# Custom-Name-Tags-SA-MP

🎯 Fitur Utama
1. Penghapusan Underscore
Nama pemain yang tadinya Jackson_Whayn akan ditampilkan sebagai Jackson Whayn (dengan spasi)
Membuat tampilan nama lebih natural dan mudah dibaca
Fungsi Nametag_RemoveUnderscore() yang otomatis memproses nama
2. Posisi Stabil
Menggunakan Attached 3D Text Label yang menempel pada player
Tidak akan turun/bergerak saat player didekati (masalah umum di nametag biasa)
Offset tinggi dapat dikustomisasi via NAMETAG_HEIGHT_OFFSET
Selalu mengikuti posisi player dengan sempurna
3. Visual Feedback Damage
Otomatis berubah warna merah saat player terkena damage
Kembali ke warna putih setelah beberapa milidetik (default 500ms)
Memberikan feedback visual yang jelas saat combat
Durasi flash dapat dikonfigurasi via DAMAGE_FLASH_TIME
4. Deteksi Damage Otomatis
Menggunakan task timer UpdateNameTag[1000] yang berjalan setiap 1 detik
Membandingkan health & armour saat ini dengan yang sebelumnya
Jika ada penurunan = player terkena damage
Tidak memerlukan callback OnPlayerTakeDamage
5. Optimasi Performa
Menggunakan YSI y_timers untuk task management yang efisien
Loop hanya untuk player yang online (GetPlayerPoolSize())
Skip player yang belum spawn (PLAYER_STATE_NONE)
Minimal overhead pada server performance
