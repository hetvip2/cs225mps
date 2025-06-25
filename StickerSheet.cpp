//sticker sheet
#include "StickerSheet.h"
#include "Image.h"

StickerSheet::StickerSheet(const Image& picture){
    base = picture;
    // top = 0;
    //count = 0;
    //stickers.clear();
    //pos.clear();
}
  /**
   * Adds a `sticker` to the StickerSheet, so that the top-left of the sticker's
   * `Image` is at `(x, y)` on the StickerSheet.
   *
   * The sticker must be added to the lowest possible layer available.
   *
   * If all available layers have been filled, a new layer must be added above
   * all existing layers.
   *
   * @param sticker The Image of the sticker.
   * @param x The x location of the sticker on the StickerSheet
   * @param y The y location of the sticker on the StickerSheet
   *
   * @return The zero-based layer index the sticker was added to.
   */
//   int StickerSheet::addSticker(Image& sticker, int x, int y){
//      if (count == stickers.size()) {
//         stickers.push_back(&sticker);
//         pos.push_back(std::make_pair(x, y));
//         // top++;
//         count++;
//         return stickers.size() - 1;
//     }
//     for (unsigned i = 0; i < stickers.size(); i++) {
//         if (stickers[i] == nullptr) {
//             stickers[i] = &sticker;
//             pos[i] = std::make_pair(x, y);
//             count++;
//             return i;
//         }
//     }
//     return -1;
// }
int StickerSheet::addSticker(Image& sticker, int x, int y) {
    for (unsigned i = 0; i < stickers.size(); i++) {
        if (stickers[i] == nullptr) {
            stickers[i] = &sticker;
            pos[i] = std::make_pair(x, y);
            return i;
        }
    }
    stickers.push_back(&sticker);
    pos.push_back(std::make_pair(x, y));
    return stickers.size() - 1;
}
  /**
   * Adds a `sticker` to the StickerSheet at layer `layer`, so that the top-left
   * of the sticker's `Image` is at `(x, y)` on the StickerSheet. If there is already
   * a sticker at `layer` it should be replaced and if `layer` is beyond the top of the
   * existing layers, add a new layer on top.
   *
   * @param sticker The Image of the sticker.
   * @param layer The layer where the sticker is to be placed.
   * @param x The x location of the sticker on the StickerSheet
   * @param y The y location of the sticker on the StickerSheet
   *
   * @return The zero-based layer index the sticker was added to.
   */
  int StickerSheet::setStickerAtLayer(Image& sticker, unsigned layer, int x, int y){
    if (layer >= stickers.size()) {
        stickers.push_back(&sticker);
        pos.push_back(std::make_pair(x, y));  
        return stickers.size() - 1;
    } else {
        stickers[layer] = &sticker;
        pos[layer] = std::make_pair(x, y);
        return layer;
    }
}
  /**
   * Changes the x and y coordinates of the Image in the specified layer.
   *
   * If the layer is invalid or does not contain a sticker, this function
   * must return false. Otherwise, this function returns true.
   *
   * @param index Zero-based layer index of the sticker.
   * @param x The new x location of the sticker on the StickerSheet
   * @param y The new y location of the sticker on the StickerSheet
   *
   * @return `true` if the translate was successful; otherwise `false`.
   */
  bool StickerSheet::translate(unsigned index, int x, int y){
    if (index >= stickers.size() || stickers[index]  == nullptr) {
        return false;
    }
    pos[index] = std::make_pair(x, y);
    return true;
  }
  /**
   * Removes the sticker at the given zero-based layer index. Make sure that the
   * other stickers don't change order.
   *
   * @param index The layer in which to delete the png
   */
//   void StickerSheet::removeSticker(unsigned index){
//     if (index < stickers.size()) {
//         //std::cout << "removing a sticker" << index << std::endl;
//         stickers[index] = nullptr;
//         pos[index] = std::make_pair(0,0);
//         count--;
//     }
//   }
void StickerSheet::removeSticker(unsigned index) {
    if (index < stickers.size()) {
        stickers[index] = nullptr;
        pos[index] = std::make_pair(0, 0);
    }
}
  /**
   * Returns a pointer to the sticker at the specified index,
   * not a copy of it. That way, the user can modify the Image.
   *
   * If the index is invalid, return NULL.
   *
   * @param index The layer in which to get the sticker.
   *
   * @return A pointer to a specific Image in the StickerSheet
   */
  Image* StickerSheet::getSticker(unsigned index){
    if (index >= stickers.size() || stickers[index] == nullptr) {
        return nullptr;
    }
    return stickers[index];
    }
  /**
   * Returns the total number of layers available on the Stickersheet.
   *
   * @return The total number of layers
   */
  int StickerSheet::layers() const{
    return stickers.size();
  }
  /**
   * Renders the whole StickerSheet on one Image and returns that Image.
   *
   * The base picture is drawn first and then each sticker is drawn in order
   * starting with layer zero (0), then layer one (1), and so on.
   *
   * If a pixel's alpha channel in a sticker is zero (0), no pixel is drawn
   * for that sticker at that pixel. If the alpha channel is non-zero, a
   * pixel is drawn. (Alpha blending is awesome, but not required.)
   *
   * The returned image always includes the full contents of the picture and
   * all stickers. It should expand in each corresponding direction if
   * stickers go beyond the edge of the picture.
   *
   * @return An Image object representing the drawn scene
   */
  Image StickerSheet::render() const {
    int xMin = 0;
    int yMin = 0;
    int xMax = base.width();
    int yMax = base.height();
    
    for (unsigned i = 0; i < stickers.size(); i++) {
        if (stickers[i] != nullptr) {
            int x = pos[i].first;
            int y = pos[i].second;
            int sw = stickers[i]->width();
            int sh = stickers[i]->height();

            xMin = std::min(xMin, x);
            yMin = std::min(yMin, y);
            xMax = std::max(xMax, static_cast<int>(x + sw));
            yMax = std::max(yMax, static_cast<int>(y + sh));
        }
    }
    int newWidth = xMax - xMin;
    int newHeight = yMax - yMin;
    //std::cout << yMax << " " << yMin << std::endl; 

    //std::cout << newWidth << " " << newHeight << std::endl; 
    Image newI(newWidth, newHeight);

    // stickers beyond image boundaries
    //expanding base image
    for (unsigned x = 0; x < base.width(); x++) {
        for (unsigned y = 0; y < base.height(); y++) {
            newI.getPixel(x - xMin, y - yMin) = base.getPixel(x, y);
        }
    }

    for (unsigned k = 0; k < stickers.size(); k++) {
        if (stickers[k] != nullptr) {
            //std::cout << " pasting sticker " << k <<std::endl;
            int x0 = pos[k].first - xMin;
            int y0 = pos[k].second - yMin;
            for (unsigned i = 0; i < stickers[k]->width(); i++) {
                for (unsigned j = 0; j < stickers[k]->height(); j++) {
                    int x = x0 + i;
                    int y = y0 + j;
                    
                    if (x >= 0 && y >= 0 && x < static_cast<int>(newI.width()) && y < static_cast<int>(newI.height())) {
                        cs225::HSLAPixel &oldPix = newI.getPixel(x, y);
                        cs225::HSLAPixel &newPix = stickers[k]->getPixel(i, j);
                        if (newPix.a != 0) {
                            oldPix = newPix;
                        }
                    }
                }
            }
        }
    }
    return newI;
}

StickerSheet::StickerSheet(const StickerSheet &other) {
    base = other.base;
    // top = other.top;
    stickers = other.stickers;
    pos = other.pos;
}

StickerSheet& StickerSheet::operator=(const StickerSheet &other) {
      if (this != &other) {
        base = other.base;
        // top = other.top;
        stickers = other.stickers;
        pos = other.pos;
    }
    return *this;
}

//StickerSheet::~StickerSheet() {}

// int main() {

//     Image base;
//     base.readFromFile("/Users/hetvipatel/Downloads/sunset-beach.jpg");
//     Image sticker1;
//     sticker1.readFromFile("/Users/hetvipatel/Downloads/sunset-beach.jpg");
//     Image sticker2;
//     sticker2.readFromFile("/Users/hetvipatel/Downloads/sunset-beach.jpg");
//     Image sticker3;
//     sticker3.readFromFile("/Users/hetvipatel/Downloads/sunset-beach.jpg");
//     StickerSheet sheet(base);

//     sheet.addSticker(sticker1, 50, 50);
//     sheet.addSticker(sticker2, 100, 100);
//     sheet.addSticker(sticker3, 150, 150);
    
//     Image finalImage = sheet.render();
//     finalImage.writeToFile("myImage.png");

//     return 0;
// }