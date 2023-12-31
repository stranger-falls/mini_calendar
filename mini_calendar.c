#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BG_WHITE        "\x1b[47m"
#define BG_MAGNETA      "\x1b[45m"
#define TEXT_YELLOW     "\x1b[33m"
#define TEXT_BLACK      "\x1b[30m"
#define TEXT_ITALIC     "\x1b[3m"
#define TEXT_BOLD       "\x1b[1m"
#define RESET_ALL       "\x1b[0m"


int is_kabise(int year, int mode) { // determine if a year is kabise (1) or not (0) and mode=0 for shamsi and mode=1 for gamari
    int i, n, divider;
    int kabise_determiner[2][11] = {{1, 5, 9, 13, 17, 22, 26, 30, 0, 0, 0,}, {2, 5, 7, 10, 13, 16, 18, 21, 24, 26, 29}};
    if (mode == 0) {
        n = 8;
        divider = 33;
    } else {
        n = 11;
        divider = 30;
    }
    for (i = 0; i < n; i++) {
        if (year % divider == kabise_determiner[mode][i]) {
            return 1;
        }
    }
    return 0;
}

int count_kabise(int year, int origin_year, int mode) { // count kabise years up to given year (current year not included)
    int kabise_count = 0, y;
    for (y = origin_year; y < year; y++) {
        kabise_count += is_kabise(y, mode);
    }
    return kabise_count;
}

int is_leap(int year) { // determine if a year is a leap year (1) or not (0)
    if (year % 100 == 0) { // end of century like 1900
        if (year % 400 == 0) { // like 2000
            return 1;
        } else {
            return 0;
        }
    } else if (year % 4 == 0) { // like 2014
        return 1;
    } else {
        return 0; // like 1933
    }
}

int count_leap(int year) { // count leap years up to given year (current year not included)
    int leap_count = 0, y;
    for (y = 1827; y < year; y++) {
        leap_count += is_leap(y);
    }
    return leap_count;
}

int count_days(int year, int month, int origin_year) { // count days until the last day of the given month
    int days_count;
    days_count = (year - origin_year) * 365 + count_kabise(year, origin_year, 0);
    days_count += month * 31;
    days_count -= ((month - 6) * (month / 7)); // adjusting for monthes with 30 days (month > 6)
    if (month == 12 && is_kabise(year, 0) == 0) {
        days_count -= 1;
    }
    return days_count;
}

int get_month_length(int year, int month) {
    int month_length = 31;
    month_length -= month / 7; // == if (month > 6) ==> month -= 1;  
    if (month == 12 && is_kabise(year, 0) == 0) {
        month_length -=1;
    }
    return month_length;
}

int goto_month_beginning(int year, int month, int origin_year) {
    int days_count;
    days_count = count_days(year, month, origin_year);
    days_count -= get_month_length(year, month) - 1; // going to the beginning of the month
    return days_count;
}

void print_month_name(int month) {
    char* month_names[12] = {"Farvardin", "Ordibehesht", "Khordad", "Tir", "Mordad", "Shahrivar", "Mehr", "Aban", "Azar", "Dey", "Bahman", "Esfand"};
    int spaces_count, i, month_name_len;

    month_name_len = strlen(month_names[month - 1]);
    spaces_count = (26 - month_name_len) / 2;
    printf(BG_WHITE TEXT_BLACK TEXT_ITALIC);
    for (i = 1; i <= spaces_count; i++) {
        printf(" ");
    }
    printf(month_names[month - 1]);
    for (i += month_name_len; i <= 26; i++) {
        printf(" ");
    }
    printf(RESET_ALL);
    printf("\n");
}

int day_of_week(int days_count) { // returns a number in range [0, 6] that 0 = Shanbe and 6 = Jome 
    days_count -= 3; // adjusting for (days_count %= 7) = 0 to be Shanbe
    days_count %= 7;
    return days_count;
}

int age_by_days(int current_year, int current_month, int today, int born_year, int born_month, int born_day) {
    int age_days; 
    age_days = count_days(current_year, current_month - 1, born_year); // from the first day of the born-year up to last month's last day 
    age_days -= count_days(born_year, born_month - 1, born_year) + born_day; // cutting out from first day of born-year up to the born-day
    age_days += today; // adding days from current-month
    return age_days;
}

void which_day_born(int born_year, int born_month, int born_day) {
    int start_of_month, origin_year = 1206, day_name;
    char * days_names[7] = {"Shanbe", "Yekshanbe", "Doshanbe", "Seshanbe", "Chaharshanbe", "Panjshanbe", "Jome"};
    start_of_month = day_of_week(goto_month_beginning(born_year, born_month, origin_year)); // month beginning day of week
    day_name = start_of_month + born_day - 1;
    day_name %= 7;
    printf("You were born on a sunny " BG_MAGNETA);
    printf(days_names[day_name]);
    printf(RESET_ALL ".\n");
}

void age_by_ymd(int current_year, int current_month, int today, int born_year, int born_month, int born_day) { // !! NOT EXACT VALUE !!
    int age_days, years, monthes, days;
    age_days = age_by_days(current_year, current_month, today, born_year, born_month, born_day);
    years = age_days / 365;
    age_days %= 365;
    age_days -= count_kabise(current_year, born_year, 0);
    monthes = age_days / 31;
    days = age_days % 31 + 3;
    printf("%d Years & %d Monthes & %d Days.\n", years, monthes, days);
}

int year_jump(int year, int mode) { // to be used in shamsi convert mode = 1 for miladi and mode = 2 for gamari
    if (mode == 1) {
        return is_leap(year);
    } else {
        return is_kabise(year, 1);
    }
}

void menu();

void back_to_menu() {
    system("cls");
    printf("Back to menu!\n-----------------------------\n");
    return menu();
}

void shamsi_to_convert(int year, int month, int day, int mode) {  // mode = 1 for miladi and mode = 2 for gamari
    int origin_year = 1206, days_count, year_length, convert_year = 1827, month_name, month_len, monthes_count;
    int additional_days = 79, base_year_length = 365;
    int convert_month_len[2][12] = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, {30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 30, 29}};

    if (mode == 2) {
        convert_year = 1242;
        additional_days = 228;
        base_year_length = 354;
    }

    days_count = goto_month_beginning(year, month, origin_year);
    days_count += day;
    days_count += additional_days; // 1206/01/01 = 1827/03/22  and  1206/01/01 = 1242/08/23
    
    year_length = base_year_length + year_jump(convert_year, mode);
    while (days_count > year_length) {
        days_count -= year_length;
        year_length = base_year_length + year_jump(convert_year, mode);
        convert_year += 1;
    }

    month_name = 0;
    month_len = convert_month_len[mode - 1][month_name]; 
    monthes_count = 1;
    while (days_count > month_len) {
        days_count -= month_len;
        month_name += 1;
        if (month_name == 1 && year_jump(convert_year, mode) == 1) {
            month_len = 29;
        } else {
            month_len = convert_month_len[mode - 1][month_name];
        }
        monthes_count += 1;
    }
    if (year_jump(convert_year - 1, mode) == 1) {
        days_count -= 1;
    }
    printf("%d %d %d\n", convert_year, monthes_count, days_count);
}

void calendar() {
    int year, month, start_of_month, day_of_month = 1, i, origin_year = 1206;

    printf("[0] Back to Menu\n--------------------------\n");
    printf("Enter Year:  ");
    scanf("%d", &year);
    if (year == 0) return back_to_menu();
    
    printf("Enter Month:  ");
    scanf("%d", &month);
    if (month == 0) return back_to_menu();

    system("cls");

    print_month_name(month);
    printf("SH  YE  DO  SE  CH  PA  JO\n");

    start_of_month = day_of_week(goto_month_beginning(year, month, origin_year)); // month beginning day of week
    for (i = 0; i < start_of_month; i++) { // printing spaces
        printf("    ");
    }
    for (i = start_of_month; i < start_of_month + get_month_length(year, month); i++) {
        if (day_of_month < 10) {
            printf(" ");
        }
        if (i % 7 == 6) { // if jome ...
            printf("%d\n", day_of_month);
        } else {
            printf("%d  ", day_of_month);
        }
        day_of_month += 1;
    }
    printf("\n--------------------------\n");
    return calendar();
}

void age() {
    int current_year, current_month, today, born_year, born_month, born_day;
    int age_days;

    printf("[0] Back to Menu\n-----------------------------\n");
    printf("Please enter today's date in format y m d (example: 1401 12 17): ");
    scanf("%d", &current_year);
    if (current_year == 0) return back_to_menu();
    scanf("%d", &current_month);
    if (current_month == 0) return back_to_menu();
    scanf("%d", &today);
    if (today == 0) return back_to_menu();

    printf("Please enter birthdate in format y m d (example: 1375 4 22): ");
    scanf("%d", &born_year);
    if (born_year== 0) return back_to_menu();
    scanf("%d", &born_month);
    if (born_month == 0) return back_to_menu();
    scanf("%d", &born_day);
    if (born_day == 0) return back_to_menu();

    system("cls");

    age_by_ymd(current_year, current_month, today, born_year, born_month, born_day);

    age_days = age_by_days(current_year, current_month, today, born_year, born_month, born_day);
    printf("%d days old\n", age_days);

    which_day_born(born_year, born_month, born_day);

    printf("-----------------------------\n");
    return age();
    
}

void conversion(int mode);

void conversion_menu() {
    int option;

    printf("[1] Shamsi to Miladi\n[2] Shamsi to Gamari\n");
    printf("-----------------------------\nSelect Option: ");
    scanf("%d", &option);

    if (option == 0) {
        system("cls");
        return back_to_menu();

    } else if (option == 1 || option == 2) {
        system("cls");
        printf("[0] Back to Menu\n-----------------------------\n");
        return conversion(option);

    } else {
        system("cls");
        printf("Invalid Input!\n-----------------------------\n");
        return conversion_menu();
    }
}

void conversion(int mode) {
    int year, month, day;

    printf("Please enter date in format y m d (example: 1369 7 19): ");
    
    scanf("%d", &year);
    if (year == 0) return back_to_menu();
    scanf("%d", &month);
    if (month == 0) return back_to_menu();
    scanf("%d", &day);
    if (day == 0) return back_to_menu();

    system("cls");

    printf("%d %d %d", year, month, day);
    printf(" " BG_WHITE TEXT_BLACK "==>" RESET_ALL " ");
    shamsi_to_convert(year, month, day, mode);

    printf("-----------------------------\n");
    printf("[0] Back to Menu\n-----------------------------\n");
    return conversion_menu();
}

void menu() {
    int choice;

    printf("[0] Quit\n[1] Calendar\n[2] Age\n[3] Conversion\n");
    printf("-----------------------------\nSelect Option:  ");
    scanf("%d", &choice);

    if (choice == 0) {
        system("cls");
        printf("Bye!");
    } else if (choice == 1) {
        system("cls");
        calendar();
    } else if (choice == 2) {
        system("cls");
        age();
    } else if (choice == 3) {
        system("cls");
        printf("[0] Back to Menu\n-----------------------------\n");
        conversion_menu();
    } else {
        system("cls");
        printf("Invalid Input!\n");
        menu();
    }
}
    
int main() {
    system("cls");
    menu();
}