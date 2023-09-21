#include "stringslib.h"

#include <stdlib.h>
#include <math.h>

/**
 * Checks if a character is present in a given string.
 *
 * @param c Character to check.
 * @param chrs String in which to check the presence of the character.
 * @return 1 if the character is present in the string, otherwise 0.
 */
int char_in_chars(char c, const char *chrs);

/**
 * Computes the number of digits in a given integer.
 *
 * @param n The integer whose length (number of digits) needs to be determined.
 * @return The number of digits in the integer.
 */
int num_length(int n);

size_t str_length(const char *str) {
    if (str == NULL) {
        return 0;
    }

    size_t count = 0;
    while(*str != '\0') {
        count++;
        str++;
    }
    return count;
}

int str_compare(const char *str1, const char *str2) {
    while (*str1 != '\0' && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

/*
 * Search for the needle in the haystack.
 * Returns a pointer to the beginning of haystack in needle,
 * or NULL if needle is not in haystack.
 */
const char *str_search(const char *needle, const char *haystack) {
    if (!needle || *needle == '\0') {
        return haystack;
    }

    while (*haystack != '\0') {
        const char *h_temp = haystack;
        const char *n_temp = needle;

        // tant que les caractères correspondent
        while (*h_temp != '\0' && *n_temp != '\0' && *h_temp == *n_temp) {
            h_temp++;
            n_temp++;
        }

        // si nous avons atteint la fin de needle, alors nous avons trouvé une occurrence
        if (*n_temp == '\0') {
            return haystack;
        }

        haystack++;
    }

    return NULL; // si on ne trouve pas needle dans haystack
}


const char *str_search_first_char(char needle, const char *haystack) {
    if (!needle || !haystack) return NULL;

    while(*haystack != '\0') {
        if(*haystack == needle) {
            return haystack;
        }
        haystack++;
    }
    return NULL;
}

const char *str_search_last_char(char needle, const char *haystack) {
    const char *last_occurrence = NULL;

    if (!needle || !haystack) return NULL;

    while (*haystack != '\0') {
        if (*haystack == needle) {
            last_occurrence = haystack;
        }
        haystack++;
    }

    return last_occurrence;
}


size_t str_prefix_accept(const char *str, const char *chars) {
    size_t count = 0;

    while (*str != '\0') {
        if (char_in_chars(*str, chars) == 0) break;

        count++;
        str++;
    }

    return count;
}

size_t str_prefix_reject(const char *str, const char *chars) {
    size_t count = 0;

    while (*str != '\0') {
        if (char_in_chars(*str, chars)) break;

        count++;
        str++;
    }

    return count;
}

int str_to_integer(const char *str) {
    if(*str < '0' || *str > '9') return 0;
    int res = 0;
    while(*str >= '0' && *str <= '9') {
        res = res * 10 + (*str - '0');
        str++;
    }
    return res;
}


int str_to_integer_ex(const char *str, const char **endptr, int base) {
    if (base < 2 || base > 36) {
        if (endptr) {
            *endptr = str; // position de départ invalide
        }
        return 0;
    }

    int result = 0;
    while (*str) {
        int value;

        if (*str >= '0' && *str <= '9') {
            value = *str - '0';
        } else if (*str >= 'a' && *str <= 'z') {
            value = *str - 'a' + 10;
        } else if (*str >= 'A' && *str <= 'Z') {
            value = *str - 'A' + 10;
        } else {
            break; // Caractère non valide
        }

        if (value >= base) {
            break; // Caractère non valide pour la base
        }

        result = result * base + value;
        str++;
    }

    if (endptr) {
        *endptr = str;
    }

    return result;
}


/*
 * Transform an integer into a string.
 * The destination buffer has a length of size bytes. If there
 * is not enough space in the buffer, the result is truncated.
 */
void str_from_integer(char *dest, size_t size, int n) {
    if (size == 0) {
        return;  // Pas d'espace pour écrire
    }

    // Si le tampon est de taille 1, nous pouvons seulement écrire le caractère '\0'
    if (size == 1) {
        dest[0] = '\0';
        return;
    }

    // Si n vaut 0, on écrit 0 et on sort directement sans passer par le calcul.
    if (n == 0) {
        dest[0] = '0';
        dest[1] = '\0';
        return;
    }

    // Si le nombre de chiffres est supérieur à la taille du tampon, avancez `n` de manière à tronquer les chiffres les moins significatifs.
    size_t len = num_length(n);
    while (len >= size) {
        n /= 10;
        len--;
    }

    size_t index = len;
    dest[index] = '\0';
    index--;

    while (n) {
        dest[index] = '0' + (n % 10);
        n /= 10;
        if (index > 0) {
            index--;
        }
    }
}

void str_copy(char *dest, size_t size, const char *src) {
    while(--size && *src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

char *str_duplicate(const char *str) {
    size_t length = str_length(str)+1;
    char *copy = (char *) malloc(length);

    if (!copy) {
        return NULL;
    }

    str_copy(copy, length,  str);
    return copy;
}

/*
 * Concatenate a string to an existing string.
 * The destination buffer has a length of size bytes. If there
 * is not enough space in the buffer, the result is truncated.
 */
void str_concat_string(char *dest, size_t size, const char *str) {
    size_t count = 0;
    while(*dest != '\0'){
        dest++;
        count++;
    }
    while(*str && count < size-1) {
        *dest = *str;
        dest++;
        str++;
        count++;
    }
    *dest = '\0';
}

/*
 * Concatenate a character to an existing string.
 * The destination buffer has a length of size bytes. If there
 * is not enough space in the buffer, the result is truncated.
 */
void str_concat_char(char *dest, size_t size, char c) {
    size_t count = 0;
    while(*dest != '\0') { // on se deplace vers le derniers element de la chaine et on incrémente de 1 a chaque fois
        dest++;
        count++;
    }
    if(count < size-1) { // Si il y a la place selon `size` pour ajouter `c`, on l'ajoute.
        *dest = c;
        dest++;
        *dest = '\0';
    }
}

/*
 * Concatenate an integer to an existing string.
 * The destination buffer has a length of size bytes. If there
 * is not enough space in the buffer, the result is truncated.
 */
void str_concat_integer(char *dest, size_t size, int n) {
    size_t str_size = num_length(n)+1; // on récupère la taille que ferrait une chaine de caractère qui contiendrait n
    char* str = (char*) malloc(str_size); // On alloue la place pour la chaine
    str_from_integer(str, str_size, n);
    str_concat_string(dest, size, str);
    free(str);
}

/*
 * Concatenate an array of strings to an existing string.
 * A separator is added between the existing string and each string in the array.
 * The end of the array is identified by NULL.
 * The destination buffer has a length of size bytes. If there
 * is not enough space in the buffer, the result is truncated.
 */
void str_concat_array(char *dest, size_t size, const char *args[], char separator) {
    if(size == 0 || dest == NULL) return;

    size_t destLen = 0; // trouver la longueur actuelle de dest
    while (dest[destLen] && destLen < size) {
        destLen++;
    }

    for(size_t i = 0; args[i] && destLen < size -1; i++) { // Itérer pour chaque string contenu dans l'array
        if(destLen > 0 && dest[destLen -1] != '\0') { // Ajouter le séparateur correspondant au début
            dest[destLen] = separator;
            destLen++;
        }

        const char* src = args[i];
        while(*src != '\0' && destLen < size-1) { // Ajouter le contenu du string.
            dest[destLen] = *src;
            destLen++;
            src++;
        }
    }

    dest[destLen] = '\0';
}

char *str_join_string(const char *str1, const char *str2, char separator) {
    size_t res_length = str_length(str1) + str_length(str2) +2;
    char* res = (char *)malloc(res_length);
    if(!res) return NULL;
    char* orig_res = res;
    while(*str1) {
        if(*str1 == '\0') break;
        *res = *str1;
        str1++;
        res++;
    }
    *res = separator;
    res++;
    while(*str2) {
        if(*str2 == '\0') break;
        *res = *str2;
        str2++;
        res++;
    }
    *res = '\0';
    return orig_res;
}

char *str_join_array(const char *args[], char separator) {

    size_t fullsize = 0;
    for(size_t i = 0; args[i]; i++) {
        fullsize += str_length(args[i]); // pour la chaine de caractère
        fullsize += 1; // pour le separator
    }
    fullsize += 1; // pour le '\0'

    char *res = (char *) malloc(fullsize);
    char *orig_res = res;
    if(args[0] == NULL) {
        *res = '\0';
        return orig_res;
    }

    const char *arg1 = args[0];
        while(*arg1) {
            if(*arg1 == '\0') break;
            *res = *arg1;
            res++;
            arg1++;
        }



    for(size_t i = 1; args[i]; i++) {
        *res = separator;
        res++;
        const char *arg = args[i];
        while(*arg) {
            if(*arg == '\0') break;
            *res = *arg;
            res++;
            arg++;
        }
    }
    *res = '\0';

    return orig_res;
}


/* ------ Fonctions externes utile pour le developpement des fonctions ci-dessus */

int char_in_chars(char c, const char *chrs) {
    while (*chrs != '\0') {
        if (c == *chrs) return 1;
        chrs++;
    }
    return 0;
}

int num_length(int n) {
    if (n == 0) return 1; // Pour traiter le cas où n=0
    return (int)floor(log10(n)) + 1;
}
