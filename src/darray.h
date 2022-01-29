/********************/
/*	DYNAMIC ARRAY	*/
/********************/
#define dynamic_array_new(type)										\
	type##_array_t *type##_array__new() {							\
		type##_array_t *res = malloc(sizeof(type##_array_t));		\
		res->len = 0;												\
		res->cap = 10;												\
		res->data = calloc(res->cap, sizeof(type##_t));				\
		return res;													\
	}																

#define dynamic_array_delete(type)							\
	void type##_array__delete(type##_array_t *arr) {		\
		arr->len = 0;										\
		arr->cap = 0;										\
		free(arr->data);									\
		free(arr);											\
	}														

#define dynamic_array_push(type)											\
	void type##_array__push(type##_array_t *arr, type##_t *value) {			\
		if (arr->len >= arr->cap) {											\
			arr->cap *= 2;													\
			arr->data = realloc(arr->data, arr->cap * sizeof(type##_t));	\
		}																	\
		arr->data[arr->len++] = *value;										\
	}																		

#define DYNAMIC_ARRAY(TYPE)			\
	typedef struct TYPE##_array {	\
		TYPE##_t *data;				\
		size_t len;					\
		size_t cap;					\
	} TYPE##_array_t;				\
	dynamic_array_new(TYPE)			\
	dynamic_array_push(TYPE)		\
	dynamic_array_delete(TYPE)		

/********************/
/*	VECTOR LIKE		*/
/********************/
#define vector__set_capacity(vec, cap)					\
	do {												\
		if ((vec)) {									\
			((size_t *)(vec))[-1] = (cap);				\
		}												\
	} while (0)											

#define vector__set_size(vec, size)						\
	do {												\
		if ((vec)) {									\
			((size_t *)(vec))[-2] = (size);				\
		}												\
	} while (0)											

#define __vector__get_address(vec)						\
	((vec) ? &(((size_t *)(vec))[-2]) : NULL)				

#define __vector__get_data_address(vec_start)			\
	((vec_start) ? (void *)&((vec_start)[2]) : NULL)	

#define vector__capacity(vec)							\
	((vec) ? ((size_t *)(vec))[-1] : (size_t)0)			

#define vector__size(vec)								\
	((vec) ? ((size_t *)(vec))[-2] : (size_t)0)			

#define VECTOR_DEFAULT_CAPACITY 10

#define vector_define_new(type, vec_name) type *vec_name;							\
	do {																			\
		const size_t default_size = sizeof(type) * VECTOR_DEFAULT_CAPACITY			\
								 	+ 2 * sizeof(size_t);							\
		size_t *tmp = malloc(default_size);											\
		vec_name = (void *)(tmp + 2);												\
		vector__set_size(vec_name, 0);												\
		vector__set_capacity(vec_name, VECTOR_DEFAULT_CAPACITY);					\
	} while (0)																		

#define vector__grow(vec, factor)													\
	do {																			\
		size_t v_new_cap = vector__capacity(vec) * factor;							\
		size_t v_new_size = sizeof(*(vec)) * v_new_cap + sizeof(size_t) * 2;		\
		size_t *tmp = __vector__get_address(vec);									\
		tmp = (void *)realloc(tmp, v_new_size);										\
		vec = __vector__get_data_address(tmp);										\
		vector__set_capacity(vec, v_new_cap);										\
	} while (0)									
	
#define vector__push(vec, value)					\
	do {											\
		size_t v_size = vector__size(vec);			\
		if (v_size >= vector__capacity(vec)) {		\
			vector__grow(vec, 2);					\
		}											\
		vec[v_size] = value;						\
		vector__set_size(vec, v_size + 1);			\
	} while (0)														

#define vector__begin(vec)									\
	((vec) ? (void*)(&(vec[0])) : NULL)						

#define vector__end(vec)									\
	((vec) ? (void*)(&(vec[vector__size(vec)])) : NULL)		

#define vector__delete(vec)									\
	do {													\
		if ((vec)) {										\
			size_t *tmp = __vector__get_address(vec);		\
			free(tmp);										\
		}													\
	} while (0)	
