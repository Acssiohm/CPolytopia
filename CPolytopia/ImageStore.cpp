#include "ImageStore.h"
#include <iostream>
#include <assert.h> 

ImageStore::~ImageStore() {
	for (Image* im : id_image) {
		if (im != nullptr) {
			SDL_assert(im->is_valid());
			delete im;
		}
	}
}