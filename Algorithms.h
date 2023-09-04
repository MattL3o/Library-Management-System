#pragma once
#include "Data_Structures.h"
template<typename type>
void swap(type& v1, type& v2) {
	type tmp = v1;
	v1 = v2;
	v2 = tmp;
}

template <typename type, typename v_type>
int partition(Linked_List<type*>& list, int low, int high, v_type(type::* fptr)(void)) {


	//Put pivot in correct position
	type* pivot = *list[high];
	int pos_off = 0;
	for (int i = high - 1; i >= low; i--) {
		if (((*list[i])->*fptr)() <= (pivot->*fptr)()) {
			pos_off++;
		}
	}
	int piv_ind = low + pos_off;
	swap(*list[high], *list[piv_ind]);
	
	int pos_l = low, pos_r = high;

	while (pos_l < piv_ind && pos_r > piv_ind) {

		while (((*list[pos_l])->*fptr)() <= (*list[piv_ind]->*fptr)()) {
			pos_l++;
		}

		while (((*list[pos_r])->*fptr)() > (*list[piv_ind]->*fptr)()) {
			pos_r--;
		}

		if (pos_l < piv_ind && pos_r > piv_ind) {
			swap(*list[pos_l], *list[pos_r]);
			pos_l++;
			pos_r--;
		}

	}
	return piv_ind;
}

template <typename type, typename v_type>
void quick_sort_sub(Linked_List<type*>& list, int start, int end, v_type(type::* fptr)(void)) {

	if (start >= end) { return; }

	int part = partition(list, start, end, fptr);
	quick_sort_sub(list, start, part - 1, fptr);
	quick_sort_sub(list, part + 1, end, fptr);


}

template <typename type, typename v_type>
Linked_List<type*>& quick_sort(Linked_List<type*>& list, v_type(type::* fptr)(void)) {

	quick_sort_sub(list, 0, list.get_size() - 1, fptr);

	return list;


}

