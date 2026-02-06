#include "date.h"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace project;

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printSeparator() {
    std::cout << std::string(60, '=') << "\n";
}

void printHeader(const std::string& title) {
    printSeparator();
    std::cout << "  " << title << "\n";
    printSeparator();
}

void calculateAge(const Date& birthDate, const Date& today) {
    int totalDays = today - birthDate;
    
    int years = 0;
    int months = 0;
    int days = 0;
    
    // Yıl hesaplama
    Date temp = birthDate;
    while (temp.get_year() < today.get_year() || 
           (temp.get_year() == today.get_year() && temp.get_month() < today.get_month()) ||
           (temp.get_year() == today.get_year() && temp.get_month() == today.get_month() && 
            temp.get_month_day() <= today.get_month_day())) {
        
        Date nextYear(temp.get_month_day(), temp.get_month(), temp.get_year() + 1);
        if (nextYear <= today) {
            years++;
            temp = nextYear;
        } else {
            break;
        }
    }
    
    // Ay hesaplama
    while (temp.get_month() < today.get_month() || 
           (temp.get_month() == today.get_month() && temp.get_month_day() <= today.get_month_day())) {
        
        int nextMonth = temp.get_month() + 1;
        int nextYear = temp.get_year();
        
        if (nextMonth > 12) {
            nextMonth = 1;
            nextYear++;
        }
        
        Date nextMonthDate(temp.get_month_day(), nextMonth, nextYear);
        if (nextMonthDate <= today) {
            months++;
            temp = nextMonthDate;
        } else {
            break;
        }
    }
    
    // Gün hesaplama
    days = today - temp;
    
    std::cout << "\n";
    printHeader("YAS HESAPLAMA SONUCU");
    std::cout << "\n  Dogum Tarihiniz  : " << birthDate << "\n";
    std::cout << "  Bugunun Tarihi   : " << today << "\n\n";
    std::cout << "  Yasiniz          : " << years << " yil, " 
              << months << " ay, " << days << " gun\n";
    std::cout << "  Toplam Gun Sayisi: " << totalDays << " gun\n";
    std::cout << "  Toplam Hafta     : " << totalDays / 7 << " hafta\n";
    std::cout << "  Toplam Ay (yakl.): " << totalDays / 30 << " ay\n";
    printSeparator();
}

void showMenu() {
    std::cout << "\n";
    printHeader("TARIH ISLEMLERI MENUSU");
    std::cout << "  1. Dogum tarihi gir ve yas hesapla\n";
    std::cout << "  2. Iki tarih arasindaki farki hesapla\n";
    std::cout << "  3. Tarihe gun ekle/cikar\n";
    std::cout << "  4. Artik yil kontrolu\n";
    std::cout << "  5. Rastgele tarih uret\n";
    std::cout << "  6. Bugunun tarihini goster\n";
    std::cout << "  0. Cikis\n";
    printSeparator();
    std::cout << "  Seciminiz: ";
}

Date getDateFromUser(const std::string& prompt) {
    std::cout << "\n  " << prompt << "\n";
    std::cout << "  Format: gg/aa/yyyy (ornek: 15/02/1998)\n";
    std::cout << "  Tarih: ";
    
    while (true) {
        try {
            int day, month, year;
            char delim1, delim2;
            
            if (!(std::cin >> day >> delim1 >> month >> delim2 >> year)) {
                clearInput();
                throw InvalidDateFormatException("Hatali giris formati");
            }
            clearInput();
            
            Date date(day, month, year);
            return date;
            
        } catch (const InvalidDayException& e) {
            std::cout << "  HATA: " << e.what() << "\n";
            std::cout << "  Lutfen gecerli bir gun giriniz (1-31 arasi): ";
        } catch (const InvalidMonthException& e) {
            std::cout << "  HATA: " << e.what() << "\n";
            std::cout << "  Lutfen gecerli bir ay giriniz (1-12 arasi): ";
        } catch (const InvalidYearException& e) {
            std::cout << "  HATA: " << e.what() << "\n";
            std::cout << "  Lutfen gecerli bir yil giriniz: ";
        } catch (const InvalidDateFormatException& e) {
            std::cout << "  HATA: " << e.what() << "\n";
            std::cout << "  Lutfen dogru formatta giriniz (gg/aa/yyyy): ";
        } catch (const std::exception& e) {
            std::cout << "  Beklenmeyen hata: " << e.what() << "\n";
            std::cout << "  Tekrar deneyin: ";
        }
    }
}

void option1_calculateBirthAge() {
    printHeader("DOGUM TARIHI VE YAS HESAPLAMA");
    
    try {
        Date birthDate = getDateFromUser("Dogum tarihinizi giriniz:");
        Date today(std::time(nullptr));
        
        if (birthDate > today) {
            std::cout << "\n  HATA: Dogum tarihi gelecekte olamaz!\n";
            std::cout << "  Girdiginiz tarih: " << birthDate << "\n";
            std::cout << "  Bugunun tarihi  : " << today << "\n";
            return;
        }
        
        calculateAge(birthDate, today);
        
        // Ek bilgiler
        std::cout << "\n  Ek Bilgiler:\n";
        std::cout << "  - " << birthDate.get_year() << " yili ";
        std::cout << (Date::isleap(birthDate.get_year()) ? "ARTIK" : "NORMAL") << " yildir.\n";
        std::cout << "  - Dogdugunuz gun: ";
        
        switch (birthDate.get_week_day()) {
            case Date::Weekday::Sunday:    std::cout << "Pazar\n"; break;
            case Date::Weekday::Monday:    std::cout << "Pazartesi\n"; break;
            case Date::Weekday::Tuesday:   std::cout << "Sali\n"; break;
            case Date::Weekday::Wednesday: std::cout << "Carsamba\n"; break;
            case Date::Weekday::Thursday:  std::cout << "Persembe\n"; break;
            case Date::Weekday::Friday:    std::cout << "Cuma\n"; break;
            case Date::Weekday::Saturday:  std::cout << "Cumartesi\n"; break;
        }
        
        std::cout << "  - Yilin " << birthDate.get_year_day() << ". gununde dogdunuz.\n";
        
        // Bir sonraki doğum günü
        Date nextBirthday(birthDate.get_month_day(), birthDate.get_month(), today.get_year());
        if (nextBirthday < today) {
            nextBirthday.set_year(today.get_year() + 1);
        }
        
        int daysUntilBirthday = nextBirthday - today;
        std::cout << "  - Bir sonraki dogum gununuze " << daysUntilBirthday << " gun kaldi!\n";
        
    } catch (const DateException& e) {
        std::cout << "\n  Tarih hatasi: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "\n  Beklenmeyen hata: " << e.what() << "\n";
    }
}

void option2_dateDifference() {
    printHeader("IKI TARIH ARASINDAKI FARK");
    
    try {
        Date date1 = getDateFromUser("Birinci tarihi giriniz:");
        Date date2 = getDateFromUser("Ikinci tarihi giriniz:");
        
        int difference = date2 - date1;
        
        std::cout << "\n";
        printSeparator();
        std::cout << "  Tarih 1   : " << date1 << "\n";
        std::cout << "  Tarih 2   : " << date2 << "\n";
        std::cout << "  Fark      : " << std::abs(difference) << " gun\n";
        std::cout << "  Hafta     : " << std::abs(difference) / 7 << " hafta, " 
                  << std::abs(difference) % 7 << " gun\n";
        
        if (difference > 0) {
            std::cout << "  Durum     : Tarih 2, Tarih 1'den " << difference << " gun sonra\n";
        } else if (difference < 0) {
            std::cout << "  Durum     : Tarih 2, Tarih 1'den " << -difference << " gun once\n";
        } else {
            std::cout << "  Durum     : Tarihler AYNI\n";
        }
        printSeparator();
        
    } catch (const DateException& e) {
        std::cout << "\n  Tarih hatasi: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "\n  Beklenmeyen hata: " << e.what() << "\n";
    }
}

void option3_addSubtractDays() {
    printHeader("TARIHE GUN EKLEME/CIKARMA");
    
    try {
        Date date = getDateFromUser("Baslangic tarihini giriniz:");
        
        std::cout << "\n  Kac gun eklemek/cikarmak istiyorsunuz?\n";
        std::cout << "  (Pozitif sayi: ekle, Negatif sayi: cikar)\n";
        std::cout << "  Gun sayisi: ";
        
        int days;
        while (!(std::cin >> days)) {
            std::cout << "  Hatali giris! Tekrar deneyin: ";
            clearInput();
        }
        clearInput();
        
        Date newDate = date + days;
        
        std::cout << "\n";
        printSeparator();
        std::cout << "  Baslangic Tarihi : " << date << "\n";
        std::cout << "  Isleme Alinan Gun: " << (days >= 0 ? "+" : "") << days << " gun\n";
        std::cout << "  Yeni Tarih       : " << newDate << "\n";
        printSeparator();
        
    } catch (const DateException& e) {
        std::cout << "\n  Tarih hatasi: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "\n  Beklenmeyen hata: " << e.what() << "\n";
    }
}

void option4_leapYearCheck() {
    printHeader("ARTIK YIL KONTROLU");
    
    std::cout << "\n  Kontrol etmek istediginiz yili giriniz: ";
    int year;
    
    while (!(std::cin >> year)) {
        std::cout << "  Hatali giris! Tekrar deneyin: ";
        clearInput();
    }
    clearInput();
    
    std::cout << "\n";
    printSeparator();
    std::cout << "  " << year << " yili: " 
              << (Date::isleap(year) ? "ARTIK YIL" : "NORMAL YIL") << "\n";
    
    if (Date::isleap(year)) {
        std::cout << "  Subat ayi: 29 gun\n";
        std::cout << "  Toplam gun: 366 gun\n";
    } else {
        std::cout << "  Subat ayi: 28 gun\n";
        std::cout << "  Toplam gun: 365 gun\n";
    }
    printSeparator();
}

void option5_randomDates() {
    printHeader("RASTGELE TARIH URETME");
    
    std::cout << "\n  Kac adet rastgele tarih uretmek istersiniz? (1-20): ";
    int count;
    
    while (!(std::cin >> count) || count < 1 || count > 20) {
        std::cout << "  Hatali giris! 1-20 arasi bir sayi giriniz: ";
        clearInput();
    }
    clearInput();
    
    std::cout << "\n";
    printSeparator();
    std::cout << "  Rastgele Uretilen Tarihler:\n\n";
    
    for (int i = 0; i < count; ++i) {
        Date randomDate = Date::random_date();
        std::cout << "  " << std::setw(2) << (i + 1) << ". " << randomDate << "\n";
    }
    printSeparator();
}

void option6_showToday() {
    printHeader("BUGUNUN TARIHI");
    
    Date today(std::time(nullptr));
    
    std::cout << "\n  Tarih        : " << today << "\n";
    std::cout << "  Yilin gunu   : " << today.get_year_day() << ". gun\n";
    std::cout << "  Artik yil mi?: " << (Date::isleap(today.get_year()) ? "Evet" : "Hayir") << "\n";
    
    // Yılın kaçta kaçı geçti
    int totalDaysInYear = Date::isleap(today.get_year()) ? 366 : 365;
    double percentage = (today.get_year_day() * 100.0) / totalDaysInYear;
    std::cout << "  Yilin %" << std::fixed << std::setprecision(1) 
              << percentage << "'i gecti\n";
    
    printSeparator();
}

int main() {
    std::cout << "\n";
    printHeader("DATE SINIFI - TARIH ISLEMLERI PROGRAMI");
    std::cout << "\n  Hosgeldiniz! Bu program tarih hesaplamalari yapmaniza yardimci olur.\n";
    
    int choice;
    
    do {
        showMenu();
        
        while (!(std::cin >> choice)) {
            std::cout << "  Hatali giris! Lutfen bir sayi giriniz: ";
            clearInput();
        }
        clearInput();
        
        std::cout << "\n";
        
        switch (choice) {
            case 1:
                option1_calculateBirthAge();
                break;
            case 2:
                option2_dateDifference();
                break;
            case 3:
                option3_addSubtractDays();
                break;
            case 4:
                option4_leapYearCheck();
                break;
            case 5:
                option5_randomDates();
                break;
            case 6:
                option6_showToday();
                break;
            case 0:
                printHeader("PROGRAM SONLANDIRILIYOR");
                std::cout << "\n  Programa katkida bulundugunuz icin tesekkurler!\n";
                std::cout << "  Gorusmek uzere...\n\n";
                break;
            default:
                std::cout << "  HATA: Gecersiz secim! Lutfen 0-6 arasi bir sayi giriniz.\n";
        }
        
        if (choice != 0) {
            std::cout << "\n  Devam etmek icin Enter'a basin...";
            std::cin.get();
        }
        
    } while (choice != 0);
    
    return 0;
}
