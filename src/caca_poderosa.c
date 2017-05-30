/*
 ============================================================================
 Name        : caca_poderosa.c
 Author      : ernesto
 Version     :
 Copyright   : a veces siento que
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdint.h>

#ifdef CACA_COMUN_LOG
#include <execinfo.h>
#endif

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#include <stdarg.h>
#endif

#define CACA_COMUN_TAM_MAX_LINEA (16*200000)
#define CACA_LOG_MAX_TAM_CADENA 2000

#define CACA_COMUN_BUF_STATICO (char[1000] ) { '\0' }

#define BITCH_VECTOR_NUM_BITS (sizeof(bitch_vector) * 8)

#define CACA_COMUN_ASSERT_DUROTE 0
#define CACA_COMUN_ASSERT_SUAVECITO 1
#define CACA_COMUN_ASSERT_NIMADRES 2

#define CACA_PODEROSA_VALOR_INVALIDO -1LL

typedef unsigned int natural;
typedef int tipo_dato;

typedef long long entero_largo;
typedef unsigned long long entero_largo_sin_signo;
typedef long long bitch_vector;

typedef enum BOOLEANOS {
	falso = 0, verdadero
} bool;

/*
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_SUAVECITO
 */
#define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_DUROTE

#define assert_timeout_dummy(condition) 0;
//static inline void caca_log_debug_dummy(const char *format, ...) { }

#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_DUROTE
#define assert_timeout(condition) assert(condition);
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_SUAVECITO
#define assert_timeout(condition) if(!(condition)){sleep(10);abort();}
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_NIMADRES
#define assert_timeout(condition) 0
#endif

#ifdef CACA_COMUN_LOG
#define caca_log_debug(formato, args...) \
		do \
		{ \
			size_t profundidad = 0; \
			void *array[CACA_LOG_MAX_TAM_CADENA]; \
 			profundidad = backtrace(array, CACA_LOG_MAX_TAM_CADENA); \
			caca_log_debug_func(formato,__FILE__, __func__, __LINE__,profundidad,##args); \
		} \
		while(0);
#else
#define caca_log_debug(formato, args...) 0;
//#define caca_log_debug caca_log_debug_dummy
#endif

#define caca_comun_max(x,y) ((x) < (y) ? (y) : (x))
#define caca_comun_min(x,y) ((x) < (y) ? (x) : (y))

void caca_log_debug_func(const char *format, ...);

#define caca_comun_mapa_bitch_checa(bits, posicion, resultado) \
        __asm__ (\
                        "xor %%rdx,%%rdx\n"\
                        "movq %[bitch_posi],%%rax\n" \
                        "movq $64,%%r8\n"\
                        "divq %%r8\n"\
                        "mov $1,%[resul]\n"\
                        "mov %%rdx,%%rcx\n"\
                        "shlq %%cl,%[resul]\n"\
                        "and (%[vectors],%%rax,8),%[resul]\n"\
                        : [resul] "=b" (resultado)\
                        : [bitch_posi] "r" (posicion), [vectors] "r" (bits)\
            :"rax","rdx","rcx","r8")

static inline void caca_comun_mapa_bitch_asigna(bitch_vector *bits,
		unsigned long posicion) {
	int idx_arreglo = 0;
	int idx_registro = 0;

	idx_arreglo = posicion / 64;
	idx_registro = posicion % 64;

	bits[idx_arreglo] |= (bitch_vector) ((bitch_vector) 1 << idx_registro);

}

static inline void caca_comun_mapa_bitch_limpia(bitch_vector *bits,
		unsigned long posicion) {
	int idx_arreglo = 0;
	int idx_registro = 0;

	idx_arreglo = posicion / 64;
	idx_registro = posicion % 64;

	bits[idx_arreglo] &= (bitch_vector) ~((bitch_vector) 1 << idx_registro);

}

void caca_comun_current_utc_time(struct timespec *ts) {

#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
#else
#ifdef CACA_COMUN_LOG
	clock_gettime(CLOCK_REALTIME, ts);
#endif
#endif

}

void caca_comun_timestamp(char *stime) {
	time_t ltime;
	struct tm result;
	long ms;
#ifndef ONLINE_JUDGE
	struct timespec spec;
#endif
	char parte_milisecundos[50];

	ltime = time(NULL );

#ifndef ONLINE_JUDGE
	localtime_r(&ltime, &result);
	asctime_r(&result, stime);
#endif

	*(stime + strlen(stime) - 1) = ' ';

#ifndef ONLINE_JUDGE
	caca_comun_current_utc_time(&spec);
	ms = round(spec.tv_nsec / 1.0e3);
#endif
	sprintf(parte_milisecundos, "%ld", ms);
	strcat(stime, parte_milisecundos);
}
void caca_log_debug_func(const char *format, ...) {

	va_list arg;
	va_list arg2;
	const char *PEDAZO_TIMESTAMP_HEADER = "tiempo: %s; ";
	const char *HEADER =
			"archivo: %s; funcion: %s; linea %d; nivel: %zd caca 8====D ";
	char formato[CACA_LOG_MAX_TAM_CADENA + sizeof(HEADER)
			+ sizeof(PEDAZO_TIMESTAMP_HEADER)] = { '\0' };
	char pedazo_timestamp[sizeof(PEDAZO_TIMESTAMP_HEADER) + 100] = { '\0' };
	char cadena_timestamp[100] = { '\0' };

	caca_comun_timestamp(cadena_timestamp);
	sprintf(pedazo_timestamp, PEDAZO_TIMESTAMP_HEADER, cadena_timestamp);

	strcpy(formato, pedazo_timestamp);
	strcat(formato, HEADER);
	strcat(formato, format);
	strcat(formato, "\n");

	va_start(arg, format);
	va_copy(arg2, arg);
	vprintf(formato, arg2);
	va_end(arg2);
	va_end(arg);
// XXX: http://stackoverflow.com/questions/1716296/why-does-printf-not-flush-after-the-call-unless-a-newline-is-in-the-format-strin
	setbuf(stdout, NULL );
}

#ifdef CACA_COMUN_LOG
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%u", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%2u",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

static char *caca_comun_arreglo_a_cadena_largo(entero_largo *arreglo, int tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%lld", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
#endif
void caca_comun_strreplace(char s[], char chr, char repl_chr) {
	int i = 0;
	while (s[i] != '\0') {
		if (s[i] == chr) {
			s[i] = repl_chr;
		}
		i++;
	}
}

static inline int caca_comun_lee_matrix_long_stdin(tipo_dato *matrix,
		int *num_filas, int *num_columnas, int num_max_filas,
		int num_max_columnas) {
	int indice_filas = 0;
	int indice_columnas = 0;
	tipo_dato numero = 0;
	char *siguiente_cadena_numero = NULL;
	char *cadena_numero_actual = NULL;
	char *linea = NULL;

	linea = calloc(CACA_COMUN_TAM_MAX_LINEA, sizeof(char));

	while (indice_filas < num_max_filas
			&& fgets(linea, CACA_COMUN_TAM_MAX_LINEA, stdin)) {
		indice_columnas = 0;
		cadena_numero_actual = linea;
		caca_comun_strreplace(linea, '\n', '\0');
		if (!strlen(linea)) {
			caca_log_debug("weird, linea vacia");
			continue;
		}
		for (siguiente_cadena_numero = linea;; siguiente_cadena_numero =
				cadena_numero_actual) {
//			caca_log_debug("el numero raw %s", linea);
			numero = strtol(siguiente_cadena_numero, &cadena_numero_actual, 10);
			if (cadena_numero_actual == siguiente_cadena_numero) {
				break;
			}
			*(matrix + indice_filas * num_max_columnas + indice_columnas) =
					numero;
//			caca_log_debug("en col %d, fil %d, el valor %lu", indice_columnas,
//					indice_filas, numero);
			indice_columnas++;
//			caca_log_debug("las columnas son %d", indice_columnas);
		}
		if (num_columnas) {
			num_columnas[indice_filas] = indice_columnas;
		}
		indice_filas++;
		caca_log_debug("las filas son %d, con clos %d", indice_filas,
				indice_columnas);
	}

	*num_filas = indice_filas;
	free(linea);
	return 0;
}

#if 1
typedef natural hm_iter;
#define HASH_MAP_VALOR_INVALIDO ((hm_iter)CACA_PODEROSA_VALOR_INVALIDO)
typedef struct hash_map_entry {
	entero_largo llave;
	entero_largo valor;
} hm_entry;
typedef struct hash_map_cubeta {
	uint64_t hash;
	hm_entry *entry;
} hm_cubeta;
typedef struct hash_map_robin_hood_back_shift {
	hm_cubeta *buckets_;
	uint64_t num_buckets_;
	uint64_t num_buckets_used_;
	uint64_t probing_min_;
	uint64_t probing_max_;
} hm_rr_bs_tabla;
int hash_map_robin_hood_back_shift_init(hm_rr_bs_tabla *ht, int num_cubetas) {
	ht->num_buckets_ = num_cubetas;
	ht->buckets_ = (hm_cubeta *) calloc(ht->num_buckets_, sizeof(hm_cubeta));
	ht->num_buckets_used_ = 0;
	ht->probing_max_ = num_cubetas;
	return 0;
}
int hash_map_robin_hood_back_shift_fini(hm_rr_bs_tabla *ht) {
	if (ht->buckets_ != NULL ) {
		for (uint32_t i = 0; i < ht->num_buckets_; i++) {
			if (ht->buckets_[i].entry != NULL ) {
				free(ht->buckets_[i].entry);
				ht->buckets_[i].entry = NULL;
			}
		}
		free(ht->buckets_);
	}
	return 0;
}
static inline int hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(
		hm_rr_bs_tabla *ht, uint64_t index_stored, uint64_t *distance) {
	hm_cubeta cubeta = ht->buckets_[index_stored];
	*distance = 0;
	if (cubeta.entry == NULL )
		return -1;
	uint64_t num_cubetas = ht->num_buckets_;
	uint64_t index_init = cubeta.hash % num_cubetas;
	if (index_init <= index_stored) {
		*distance = index_stored - index_init;
	} else {
		*distance = index_stored + (num_cubetas - index_init);
	}
	return 0;
}
hm_iter hash_map_robin_hood_back_shift_obten(hm_rr_bs_tabla *ht,
		const entero_largo key, entero_largo *value) {
	uint64_t num_cubetas = ht->num_buckets_;
	uint64_t prob_max = ht->probing_max_;
//	uint64_t hash = hash_function_caca(key);
	uint64_t hash = key % num_cubetas;
	uint64_t index_init = hash;
	uint64_t probe_distance = 0;
	uint64_t index_current;
	bool found = falso;
	uint32_t i;
	*value = HASH_MAP_VALOR_INVALIDO;
	for (i = 0; i < num_cubetas; i++) {
		index_current = (index_init + i) % num_cubetas;
		hm_entry *entrada = ht->buckets_[index_current].entry;
		if (entrada == NULL ) {
			break;
		}
		hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(ht,
				index_current, &probe_distance);
		if (i > probe_distance) {
			break;
		}
		if (entrada->llave == key) {
			*value = entrada->valor;
			found = verdadero;
			break;
		}
	}
	if (found)
		return index_current;
	return HASH_MAP_VALOR_INVALIDO ;
}
hm_iter hash_map_robin_hood_back_shift_pon(hm_rr_bs_tabla *ht, entero_largo key,
		entero_largo value, bool *nuevo_entry) {
	uint64_t num_cubetas = ht->num_buckets_;
	uint64_t prob_max = ht->probing_max_;
	uint64_t prob_min = ht->probing_min_;
	hm_cubeta *cubetas = ht->buckets_;
	*nuevo_entry = verdadero;
	if (ht->num_buckets_used_ == num_cubetas) {
		*nuevo_entry = falso;
		return HASH_MAP_VALOR_INVALIDO ;
	}
	ht->num_buckets_used_ += 1;
//	uint64_t hash = hash_function_caca(key);
	uint64_t hash = key % num_cubetas;
	uint64_t index_init = hash;
	hm_entry *entry = (hm_entry *) calloc(1, sizeof(hm_entry));
	entry->llave = key;
	entry->valor = value;
	uint64_t index_current = index_init % num_cubetas;
	uint64_t probe_current = 0;
	uint64_t probe_distance;
	hm_entry *entry_temp;
	uint64_t hash_temp;
	uint64_t i;
	for (i = 0; i < num_cubetas; i++) {
		index_current = (index_init + i) % num_cubetas;
		hm_cubeta *cubeta = cubetas + index_current;
		if (cubeta->entry == NULL ) {
			cubeta->entry = entry;
			cubeta->hash = hash;
			if (index_current > prob_max) {
				ht->probing_max_ = index_current;
			}
			if (index_current < prob_min) {
				ht->probing_min_ = index_current;
			}
			break;
		} else {
			if (cubeta->entry->llave == key) {
				free(entry);
				*nuevo_entry = falso;
				break;
			}
			hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(ht,
					index_current, &probe_distance);
			if (probe_current > probe_distance) {
				// Swapping the current bucket with the one to insert
				entry_temp = cubeta->entry;
				hash_temp = cubeta->hash;
				cubeta->entry = entry;
				cubeta->hash = hash;
				entry = entry_temp;
				hash = hash_temp;
				probe_current = probe_distance;
			}
		}
		probe_current++;
	}
	return index_current;
}
int hash_map_robin_hood_back_shift_borra(hm_rr_bs_tabla *ht,
		const tipo_dato key) {
	uint64_t num_cubetas = ht->num_buckets_;
	uint64_t prob_max = ht->probing_max_;
	uint64_t prob_min = ht->probing_max_;
	uint64_t hash = key % num_cubetas;
	uint64_t index_init = hash;
	bool found = falso;
	uint64_t index_current = 0;
	uint64_t probe_distance = 0;
	hm_entry *entrada = NULL;
	for (uint64_t i = 0; i < num_cubetas; i++) {
		index_current = (index_init + i) % num_cubetas;
		entrada = ht->buckets_[index_current].entry;
		hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(ht,
				index_current, &probe_distance);
		if (entrada == NULL || i > probe_distance) {
			break;
		}
		if (entrada->llave == key) {
			found = verdadero;
			break;
		}
	}
	if (found) {
		free(entrada);
		uint64_t i = 1;
		uint64_t index_previous = 0, index_swap = 0;
		for (i = 1; i < num_cubetas; i++) {
			index_previous = (index_current + i - 1) % num_cubetas;
			index_swap = (index_current + i) % num_cubetas;
			hm_cubeta *cubeta_swap = ht->buckets_ + index_swap;
			hm_cubeta *cubeta_previous = ht->buckets_ + index_previous;
			if (cubeta_swap->entry == NULL ) {
				cubeta_previous->entry = NULL;
				break;
			}
			uint64_t distance;
			if (hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(
					ht, index_swap, &distance) != 0) {
				fprintf(stderr, "Error in FillDistanceToInitIndex()");
			}
			if (!distance) {
				cubeta_previous->entry = NULL;
				break;
			}
			cubeta_previous->entry = cubeta_swap->entry;
			cubeta_previous->hash = cubeta_swap->hash;
		}
		if (i < num_cubetas) {
			if (index_previous == prob_min) {
				prob_min++;
				if (prob_min >= num_cubetas) {
					prob_min = 0;
				} else {
					while (prob_min < num_cubetas
							&& ht->buckets_[prob_min].entry) {
						prob_min++;
					}
					if (prob_min >= num_cubetas) {
						prob_min = num_cubetas;
					}
				}
				ht->probing_min_ = prob_min;
			}
			if (index_previous == prob_max) {
				prob_max--;
				if (prob_max >= num_cubetas) {
					prob_max = num_cubetas;
				} else {
					while (((int64_t) prob_max) >= 0
							&& ht->buckets_[prob_max].entry) {
						prob_max--;
					}
					if (prob_max >= num_cubetas) {
						prob_max = 0;
					}
				}
				ht->probing_max_ = prob_max;
			}
		}
		ht->num_buckets_used_--;
		return 0;
	}
	return 1;
}
static inline int hash_map_robin_hood_back_shift_indice_inicio(
		hm_rr_bs_tabla *ht) {
	return ht->probing_min_;
}
static inline int hash_map_robin_hood_back_shift_indice_final(
		hm_rr_bs_tabla *ht) {
	return ht->probing_max_;
}
static inline bool hash_map_robin_hood_back_shift_indice_existe(
		hm_rr_bs_tabla *ht, hm_iter indice) {
	return !!ht->buckets_[indice].entry;
}
static inline entero_largo hash_map_robin_hood_back_shift_indice_obten_llave(
		hm_rr_bs_tabla *ht, hm_iter indice) {
	assert_timeout(indice <= ht->probing_max_ && indice >= ht->probing_min_);
	hm_entry *entrada = ht->buckets_[indice].entry;
	assert_timeout(entrada);
	return entrada->llave;
}
static inline void hash_map_robin_hood_back_shift_indice_pon_valor(
		hm_rr_bs_tabla *ht, hm_iter indice, entero_largo valor) {
	assert_timeout(indice <= ht->probing_max_ && indice >= ht->probing_min_);
	hm_entry *entrada = ht->buckets_[indice].entry;
	assert_timeout(entrada);
	entrada->valor = valor;
}
int hash_map_robin_hood_back_shift_indice_borra(hm_rr_bs_tabla *ht,
		hm_iter indice) {
	assert_timeout(indice <= ht->probing_max_ && indice >= ht->probing_min_);
	uint64_t num_cubetas = ht->num_buckets_;
	uint64_t prob_max = ht->probing_max_;
	uint64_t prob_min = ht->probing_max_;
	uint64_t index_current = indice;
	hm_entry *entrada = ht->buckets_[index_current].entry;
	assert_timeout(entrada);
	free(entrada);
	uint64_t i = 1;
	uint64_t index_previous = 0, index_swap = 0;
	for (i = 1; i < num_cubetas; i++) {
		index_previous = (index_current + i - 1) % num_cubetas;
		index_swap = (index_current + i) % num_cubetas;
		hm_cubeta *cubeta_swap = ht->buckets_ + index_swap;
		hm_cubeta *cubeta_previous = ht->buckets_ + index_previous;
		if (cubeta_swap->entry == NULL ) {
			cubeta_previous->entry = NULL;
			break;
		}
		uint64_t distance;
		if (hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(ht,
				index_swap, &distance) != 0) {
			fprintf(stderr, "Error in FillDistanceToInitIndex()");
		}
		if (!distance) {
			cubeta_previous->entry = NULL;
			break;
		}
		cubeta_previous->entry = cubeta_swap->entry;
		cubeta_previous->hash = cubeta_swap->hash;
	}
	if (i < num_cubetas) {
		if (index_previous == prob_min) {
			prob_min++;
			if (prob_min >= num_cubetas) {
				prob_min = 0;
			} else {
				while (prob_min < num_cubetas && ht->buckets_[prob_min].entry) {
					prob_min++;
				}
				if (prob_min >= num_cubetas) {
					prob_min = num_cubetas;
				}
			}
			ht->probing_min_ = prob_min;
		}
		if (index_previous == prob_max) {
			prob_max--;
			if (prob_max >= num_cubetas) {
				prob_max = num_cubetas;
			} else {
				while (((int64_t) prob_max) >= 0 && ht->buckets_[prob_max].entry) {
					prob_max--;
				}
				if (prob_max >= num_cubetas) {
					prob_max = 0;
				}
			}
			ht->probing_max_ = prob_max;
		}
	}
	ht->num_buckets_used_--;
	return 0;
}
static inline entero_largo hash_map_robin_hood_back_shift_indice_obten_valor(
		hm_rr_bs_tabla *ht, hm_iter indice) {
	assert_timeout(indice <= ht->probing_max_ && indice >= ht->probing_min_);
	hm_entry *entrada = ht->buckets_[indice].entry;
	assert_timeout(entrada);
	return entrada->valor;
}
static inline bool hash_map_robin_hood_back_shift_esta_vacio(hm_rr_bs_tabla *ht) {
	assert_timeout(ht->num_buckets_used_ <= ht->num_buckets_);
	return !ht->num_buckets_used_;
}
#endif

#define CACA_PODEROSA_MAX_NUM 1000000
#define CACA_PODEROSA_MAX_NUMEROS 200000
natural ocurrencias[CACA_PODEROSA_MAX_NUM + 2] = { 0 };

#define caca_poderosa_quita_caca(numero) \
        __asm__ (\
                        "mov (%[ocurr],%[num],4),%%eax\n"\
                        "mov $2,%%edx\n"\
                        "mul %%edx\n"\
                        "sub $1,%%eax\n"\
                        "mov $-1,%%edx\n"\
                        "imul %%edx\n"\
                        "imul %[num]\n"\
                        "lea %[resu_low],%%esi\n"\
                        "add %%eax,%[resu_low]\n"\
                        "adc %%edx,4(%%esi)\n"\
                        "subl $1,(%[ocurr],%[num],4)\n"\
:[resu_low] "=m" (mo_mada_resultado)\
: [num] "r" (numero), [ocurr] "r" (ocurrencias)\
            :"eax","edx","esi")

#define caca_poderosa_anade_caca(numero) \
        __asm__ (\
                        "mov (%[ocurr],%[num],4),%%eax\n"\
                        "mov $2,%%edx\n"\
                        "mul %%edx\n"\
                        "add $1,%%eax\n"\
                        "mul %[num]\n"\
                        "lea %[resu_low],%%esi\n"\
                        "add %%eax,%[resu_low]\n"\
                        "adc %%edx,4(%%esi)\n"\
                        "addl $1,(%[ocurr],%[num],4)\n"\
:[resu_low] "=m" (mo_mada_resultado)\
: [num] "r" (numero), [ocurr] "r" (ocurrencias)\
            :"eax","edx","esi")

#if 1
typedef enum mo_mada_tipo_query {
	mo_mada_actualizacion = 'U', mo_mada_consulta = 0
} mo_mada_tipo_query;

typedef struct mo_mada {
	mo_mada_tipo_query tipo;
	natural idx_query;
	natural intervalo_idx_ini;
	natural intervalo_idx_fin;
	natural orden;
	entero_largo resulcaca;
} mo_mada;

natural mo_mada_tam_bloque = 0;
entero_largo mo_mada_resultado = 0;

int mo_mada_ord_bloque(const void *pa, const void *pb) {
	int res = 0;
	mo_mada *a = (mo_mada *) pa;
	mo_mada *b = (mo_mada *) pb;
	natural idx_bloque_a = a->intervalo_idx_ini / mo_mada_tam_bloque;
	natural idx_bloque_b = b->intervalo_idx_ini / mo_mada_tam_bloque;
	natural idx_fin_a = a->intervalo_idx_fin;
	natural idx_fin_b = b->intervalo_idx_fin;
	natural idx_ini_a = a->intervalo_idx_ini;
	natural idx_ini_b = b->intervalo_idx_ini;

	if (idx_bloque_a != idx_bloque_b) {
		res = idx_bloque_a - idx_bloque_b;
	} else {
		if (idx_fin_a != idx_fin_b) {
			res = idx_fin_a - idx_fin_b;
		} else {
			if (idx_ini_a != idx_ini_b) {
				res = idx_ini_a - idx_ini_b;
			} else {
				res = a->orden - b->orden;
			}
		}
	}

	return res;
}

int mo_mada_ord_idx_query(const void *pa, const void *pb) {
	int res = 0;
	mo_mada *a = (mo_mada *) pa;
	mo_mada *b = (mo_mada *) pb;

	res = a->idx_query - b->idx_query;
	return res;
}

#define mo_mada_fn_ord_mo mo_mada_ord_bloque
#define mo_mada_fn_ord_idx mo_mada_ord_idx_query
#define mo_mada_fn_anade_caca caca_poderosa_anade_caca
#define mo_mada_fn_quita_caca caca_poderosa_quita_caca

static inline mo_mada *mo_mada_core(mo_mada *consultas, tipo_dato *numeros,
		natural num_consultas, natural num_numeros) {
	natural idx_izq_act = 0;
	natural idx_der_act = 0;
	mo_mada_tam_bloque = ceil(sqrt(num_numeros));
	caca_log_debug("total de nums %u, tam bloq %u\n", num_numeros,
			mo_mada_tam_bloque);

	qsort(consultas, num_consultas, sizeof(mo_mada), mo_mada_fn_ord_mo);

	idx_izq_act = idx_der_act = (consultas)->intervalo_idx_ini;

	caca_log_debug("anadiendo inicialmente %u\n", numeros[idx_izq_act]);
	mo_mada_fn_anade_caca((numeros[idx_izq_act]));

	caca_log_debug("puta mierda %lld\n", mo_mada_resultado);

	for (int i = 0; i < num_consultas; i++) {
		natural consul_idx_izq = (consultas + i)->intervalo_idx_ini;
		natural consul_idx_der = (consultas + i)->intervalo_idx_fin;

		if ((consultas + i)->tipo == mo_mada_actualizacion) {
			continue;
		}

#if 0
		assert_timeout(
				ceil(abs((int )idx_izq_act - (int )consul_idx_izq))
				<= mo_mada_tam_bloque * 2);
#endif

		caca_log_debug("vamos a bailar %u-%u\n", consul_idx_izq, consul_idx_der);

		caca_log_debug("disminu izq act %u a izq consul %u\n", idx_izq_act,
				consul_idx_izq);
		while (idx_izq_act > consul_idx_izq) {
			idx_izq_act--;
			mo_mada_fn_anade_caca(numeros[idx_izq_act]);
			caca_log_debug("puta mierda %lld\n", mo_mada_resultado);
		}

		caca_log_debug("aumen der act %u a der consul %u\n", idx_der_act,
				consul_idx_der);
		while (idx_der_act < consul_idx_der) {
			idx_der_act++;
			mo_mada_fn_anade_caca(numeros[idx_der_act]);
			caca_log_debug("puta mierda %lld\n", mo_mada_resultado);
		}

		caca_log_debug("aumen izq act %u a izq consul %u\n", idx_izq_act,
				consul_idx_izq);
		while (idx_izq_act < consul_idx_izq) {
			mo_mada_fn_quita_caca(numeros[idx_izq_act]);
			caca_log_debug("perron %lld\n", mo_mada_resultado);
			idx_izq_act++;
		}

		caca_log_debug("disminu der act %u a der consul %u\n", idx_der_act,
				consul_idx_der);
		while (idx_der_act > consul_idx_der) {
			mo_mada_fn_quita_caca(numeros[idx_der_act]);
			caca_log_debug("perron1 %lld\n", mo_mada_resultado);
			idx_der_act--;
		}

		caca_log_debug("el conteo uniq de la consul %u es %lld\n", i,
				mo_mada_resultado);
		(consultas + i)->resulcaca = mo_mada_resultado;
	}

	qsort(consultas, num_consultas, sizeof(mo_mada), mo_mada_fn_ord_idx);
	return consultas;
}

#endif

#if 1
#define BIT_CH_VALOR_INVALIDO ULLONG_MAX
#define BIT_CH_MAX_NODOS CACA_PODEROSA_MAX_NUMEROS

typedef struct bit_ch {
	natural num_nodos_bit_ch;
	entero_largo_sin_signo nodos_bit_ch[BIT_CH_MAX_NODOS + 2];
} bit_ch;

static inline void bit_ch_init(bit_ch *bit, tipo_dato valor_inicial,
		natural num_cacas, tipo_dato *numeros) {

	bit->num_nodos_bit_ch = num_cacas;
}

#if 1
static inline void bit_ch_aumenta(bit_ch *bit, tipo_dato nuevo_valor,
		natural idx) {
	entero_largo_sin_signo *nodos = bit->nodos_bit_ch;
	caca_log_debug("q verga actualizando %u con %d", idx, nuevo_valor);
	for (natural i = idx; i <= bit->num_nodos_bit_ch; i += (i & (-i))) {
		entero_largo ass = nodos[i];
		caca_log_debug("actualizando caca %u con %d antes %lld", i, nuevo_valor,
				nodos[i]);
		ass += nuevo_valor;
		nodos[i] = ass;
		caca_log_debug("actualizado caca %u aora %lld", i, nodos[i]);
	}
}

static inline tipo_dato bit_ch_consulta(bit_ch *bit, natural idx) {
	entero_largo_sin_signo *nodos = bit->nodos_bit_ch;
	entero_largo_sin_signo res = 0;
	for (int i = idx; i > 0; i -= (i & (-i))) {
		assert_timeout(i<BIT_CH_MAX_NODOS + 2);
		caca_log_debug("consultando caca %u tiene %lld", i, nodos[i]);
		res += nodos[i];
	}
	caca_log_debug("regresando %lld", res);
	return res;
}

static inline void bit_ch_aumenta_rango(bit_ch *bit, tipo_dato nuevo_valor,
		natural idx_ini, natural idx_fin) {
	caca_log_debug("aumentando de %u a %u con valor %lld", idx_ini, idx_fin,
			nuevo_valor);

	bit_ch_aumenta(bit, nuevo_valor, idx_ini);
	bit_ch_aumenta(bit, -nuevo_valor, idx_fin + 1);
}

static inline void bit_ch_aumenta_rango_consulta_rango(bit_ch *bit_puto,
		bit_ch *bit_aux, natural idx_ini, natural idx_fin,
		tipo_dato nuevo_valor) {

	caca_log_debug("aumentando de %u a %u con valor %lld para consulta rango",
			idx_ini, idx_fin, nuevo_valor);

	bit_ch_aumenta_rango(bit_puto, nuevo_valor, idx_ini, idx_fin);
	caca_log_debug(
			"actualizando para consulta rango inicial %d con %lld (%lld * %u)",
			idx_ini, nuevo_valor * (idx_ini - 1), nuevo_valor, idx_ini - 1);
	bit_ch_aumenta(bit_aux, nuevo_valor * (idx_ini - 1), idx_ini);
	caca_log_debug(
			"actualizando para consulta rango final %d con %lld(%lld,%u)",
			idx_fin + 1, -nuevo_valor * idx_fin, -nuevo_valor, idx_fin);
	bit_ch_aumenta(bit_aux, -nuevo_valor * idx_fin, idx_fin + 1);
}

static inline tipo_dato bit_ch_consulta_rango_actualizado_rango(
		bit_ch *bit_puto, bit_ch *bit_aux, natural idx) {
	tipo_dato resul = 0;
	tipo_dato valor_puto = 0;
	tipo_dato valor_aux = 0;

	valor_puto = bit_ch_consulta(bit_puto, idx);

	caca_log_debug("el valor putual en %u es %lld, aportara %lld (%lld * %u)",
			idx, valor_puto, valor_puto * idx, valor_puto, idx);

	valor_aux = bit_ch_consulta(bit_aux, idx);

	caca_log_debug("el valor aux en %u es %lld", idx, valor_puto);

	resul = valor_puto * idx - valor_aux;

	caca_log_debug("la suma acumulacaca hasta %u es %lld", idx, resul);

	return resul;
}

static inline tipo_dato bit_ch_consulta_rango(bit_ch *bit_puto, bit_ch *bit_aux,
		natural idx_ini, natural idx_fin) {
	tipo_dato resul = 0;
	tipo_dato resul_idx_ini = 0;
	tipo_dato resul_idx_fin = 0;

	resul_idx_ini = bit_ch_consulta_rango_actualizado_rango(bit_puto, bit_aux,
			idx_ini - 1);
	caca_log_debug("la suma acumulacaca ini hasta %u es %lld", idx_ini,
			resul_idx_ini);

	resul_idx_fin = bit_ch_consulta_rango_actualizado_rango(bit_puto, bit_aux,
			idx_fin);
	caca_log_debug("la suma acumulacaca fin hasta %u es %lld", idx_fin,
			resul_idx_fin);

	resul = resul_idx_fin - resul_idx_ini;

	caca_log_debug("la suma acumulacaca del rango %u-%u es %lld", idx_ini,
			idx_fin, resul);

	return resul;
}
#endif

#endif

#define CACA_PODEROSA_MAX_CONSULS 200000
#define CACA_PODEROSA_MAX_NUMS 200000

hm_rr_bs_tabla *tablon = &(hm_rr_bs_tabla ) { 0 };
mo_mada consultas[CACA_PODEROSA_MAX_CONSULS] = { 0 };
tipo_dato numeros[CACA_PODEROSA_MAX_NUMS + 2] = { 0 };
natural consultas_tam = 0;
natural numeros_tam = 0;
natural consultas_interfalo[CACA_PODEROSA_MAX_CONSULS][2] = { 0 };
bit_ch *biatch = &(bit_ch ) { 0 };

int caca_poderosa_ord_limite_der(const void *pa, const void *pb) {
	int res = 0;
	mo_mada *a = (mo_mada *) pa;
	mo_mada *b = (mo_mada *) pb;

	if (a->intervalo_idx_fin == b->intervalo_idx_fin) {
		if (a->intervalo_idx_ini == b->intervalo_idx_ini) {
			res = a->orden - b->orden;
		} else {
			res = a->intervalo_idx_ini - b->intervalo_idx_ini;
		}
	} else {
		res = a->intervalo_idx_fin - b->intervalo_idx_fin;
	}
	return res;
}

int caca_poderosa_ord_idx_query(const void *pa, const void *pb) {
	int res = 0;
	mo_mada *a = (mo_mada *) pa;
	mo_mada *b = (mo_mada *) pb;

	res = a->idx_query - b->idx_query;
	return res;
}

static inline void caca_poderosa_core() {
	entero_largo incremento_actual = 0;
	entero_largo detrimento_actual = 0;
	mo_mada *consul_act = NULL;
	natural idx_consul_act = 0;

	hash_map_robin_hood_back_shift_init(tablon, CACA_PODEROSA_MAX_NUMS << 1);
	for (int i = 1; i <= numeros_tam; i++) {
		bool nueva_mierda = falso;
		hash_map_robin_hood_back_shift_pon(tablon, numeros[i], 0,
				&nueva_mierda);
		ocurrencias[numeros[i]] = -1;
	}

	caca_log_debug("inicializando bitch");
	bit_ch_init(biatch, 0, numeros_tam, numeros);
	for (int i = 1; i <= numeros_tam; i++) {
		ocurrencias[numeros[i]] += 1;
		bit_ch_aumenta(biatch, (2 * ocurrencias[numeros[i]] + 1) * numeros[i],
				i);
	}

	qsort(consultas, consultas_tam, sizeof(mo_mada),
			caca_poderosa_ord_limite_der);

	consul_act = consultas + idx_consul_act;
	caca_log_debug("la consul inicial %u-%u pos %u resulta %lld",
			consul_act->intervalo_idx_ini, consul_act->intervalo_idx_fin,
			idx_consul_act, consul_act->resulcaca);
	for (int i = 1; i <= numeros_tam; i++) {
		natural num_actual = numeros[i];
		natural ocurrencia_actual = ocurrencias[num_actual];
		caca_log_debug("procesando num %u pos %u ocurrencias %u", num_actual, i,
				ocurrencia_actual);

		while (consul_act->intervalo_idx_ini == i
				&& idx_consul_act < consultas_tam) {
			natural idx_lim_der = consul_act->intervalo_idx_fin;
			entero_largo_sin_signo resul = 0;

			resul = bit_ch_consulta(biatch, idx_lim_der);

			consul_act->resulcaca = resul;
			caca_log_debug("la consul %u-%u pos %u resulta %lld",
					consul_act->intervalo_idx_ini,
					consul_act->intervalo_idx_fin, idx_consul_act,
					consul_act->resulcaca);

			if (ocurrencia_actual) {
				bit_ch_aumenta(biatch, -2 * num_actual, 1);
			}
			consul_act = consultas + ++idx_consul_act;
		}
	}

	qsort(consultas, consultas_tam, sizeof(mo_mada),
			caca_poderosa_ord_idx_query);

	for (int i = 0; i < consultas_tam; i++) {
		mo_mada *consul_act = (consultas + i);
//		printf("%lld\n", consul_act->resulcaca);
	}
}

static inline void caca_poderosa_main() {
	scanf("%u %u\n", &numeros_tam, &consultas_tam);
	caca_log_debug("num nums %u num consuls %u", numeros_tam, consultas_tam);
	caca_comun_lee_matrix_long_stdin((tipo_dato *) numeros + 1, &(int ) { 0 },
			NULL, 1, numeros_tam);
	caca_comun_lee_matrix_long_stdin((tipo_dato *) consultas_interfalo,
			&(int ) { 0 }, NULL, consultas_tam, 2);
	for (int i = 0; i < consultas_tam; i++) {
		natural *consul_inter_act = consultas_interfalo[i];
		mo_mada *consul_act = (consultas + i);
		consul_act->orden = consul_act->idx_query = i;
		consul_act->intervalo_idx_ini = consul_inter_act[0];
		consul_act->intervalo_idx_fin = consul_inter_act[1];
		caca_log_debug("se q t %u: %u-%u", i, consul_act->intervalo_idx_ini,
				consul_act->intervalo_idx_fin);
	}
	caca_poderosa_core();
}

int main(void) {
	caca_poderosa_main();
	return EXIT_SUCCESS;
}
