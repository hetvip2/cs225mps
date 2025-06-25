#include "Image.h"

    Image:: Image() : PNG() {}

    /**
      * Creates an Image of the specified dimensions.
      * @param width Width of the new image.
      * @param height Height of the new image.
      */
    Image:: Image(unsigned int width, unsigned int height) : PNG(width, height) {}

    /**
     * Lighten an Image by increasing the luminance of every pixel by 0.1.
     * This function ensures that the luminance remains in the range [0, 1].
    **/
    void Image::lighten(){
        lighten(0.1);
    }
    /**
     * Lighten an Image by increasing the luminance of every pixel by `amount`.
     * This function ensures that the luminance remains in the range [0, 1].
     *
     * @param amount The desired increase in luminance.
    **/
    void Image::lighten(double amount){
        for (unsigned i = 0; i < width(); i++){
            for (unsigned j = 0; j < height(); j++){
                cs225::HSLAPixel &pixel = getPixel(i, j);
                pixel.l += amount;
                if (pixel.l > 1.0) {
                    pixel.l = 1.0;
                }
            }
        }
    }

    /**
     * Darken an Image by decreasing the luminance of every pixel by 0.1.
     * This function ensures that the luminance remains in the range [0, 1].
    **/
    void Image::darken(){
        darken(0.1);
    }

    /**
     * Darkens an Image by decreasing the luminance of every pixel by `amount`.
     * This function ensures that the luminance remains in the range [0, 1].
     *
     * @param amount The desired decrease in luminance.
    **/
    void Image::darken(double amount){
        for (unsigned i = 0; i < width(); i++){
            for (unsigned j = 0; j < height(); j++){
                cs225::HSLAPixel &pixel = getPixel(i, j);
                pixel.l -= amount;
                if (pixel.l > 1.0) {
                    pixel.l = 1.0;
                } else if (pixel.l < 0.0) {
                    pixel.l = 0.0;
                }
            }
        }
    }
    /**
     * Saturates an Image by increasing the saturation of every pixel by 0.1.
     * This function ensures that the saturation remains in the range [0, 1].
    **/
    void Image::saturate(){
        saturate(0.1);
    }
    /**
     * Saturates an Image by increasing the saturation of every pixel by `amount`.
     * This function ensures that the saturation remains in the range [0, 1].
     *
     * @param amount The desired increase in saturation.
    **/
    void Image::saturate(double amount){
        for (unsigned i = 0; i < width(); i++){
            for (unsigned j = 0; j < height(); j++){
                cs225::HSLAPixel &pixel = getPixel(i, j);
                pixel.s += amount;
                if (pixel.s > 1.0) {
                    pixel.s = 1.0;
                }
            }
        }
    }
    /**
     * Desaturates an Image by decreasing the saturation of every pixel by 0.1.
     * This function ensures that the saturation remains in the range [0, 1].
    **/
    void Image::desaturate(){
        desaturate(0.1);
    }
    /**
     * Desaturates an Image by decreasing the saturation of every pixel by `amount`.
     * This function ensures that the saturation remains in the range [0, 1].
     *
     * @param amount The desired decrease in saturation.
    **/
    void Image::desaturate(double amount){
        for (unsigned i = 0; i < width(); i++){
            for (unsigned j = 0; j < height(); j++){
                cs225::HSLAPixel &pixel = getPixel(i, j);
                pixel.l -= amount;
                if (pixel.s > 1.0) {
                    pixel.s = 1.0;
                }
            }
        }
    }

    /**
     * Turns the image grayscale.
    **/
    void Image::grayscale(){
        for (unsigned i = 0; i < width(); i++){
            for (unsigned j = 0; j < height(); j++){
                cs225::HSLAPixel &pixel = getPixel(i, j);
                pixel.s = 0;
            }
        }
    }
    /**
     * Rotates the color wheel by `degrees`. Rotating in a positive direction increases the degree of the hue.
     * This function ensures that the hue remains in the range [0, 360].
     *
     * @param degrees The desired amount of rotation.
    **/
    void Image:: rotateColor(double degrees){
        for (unsigned i = 0; i < width(); i++){
            for (unsigned j = 0; j < height(); j++){
                cs225::HSLAPixel &pixel = getPixel(i, j);
                pixel.h += degrees;
                while (pixel.h >= 360.0) {
                    pixel.h -= 360.0;
                }
                while (pixel.h < 0.0) {
                    pixel.h += 360.0;
                }
            }
        }
    }
    /**
     * Illinify the image.
    **/
    void Image::illinify() {
        const double orange = 11.0;
        const double blue = 216.0;
        for (unsigned i = 0; i < width(); i++) {
            for (unsigned j = 0; j < height(); j++) {
                cs225::HSLAPixel &pixel = getPixel(i, j);
                if (std::abs(pixel.h - orange) < std::abs(pixel.h - blue)) {
                    pixel.h = orange;
                } else {
                    pixel.h = blue;
                }
            }
        }
    }
    /**
     * Scale the Image by a given `factor`.
     *
     * For example:
     * - A `factor` of 1.0 does not change the image.
     * - A `factor` of 0.5 results in an image with half the width and half the height.
     * - A `factor` of 2 results in an image with twice the width and twice the height.
     *
     * This function both resizes the Image and scales the contents.
     *
     * @param factor Scale factor.
    **/
    void Image::scale(double factor) {
        unsigned newWidth = width() * factor;
        unsigned newHeight = height() * factor;

        Image scaledImage(newWidth, newHeight);

        for (unsigned i = 0; i < newWidth; i++) {
            for (unsigned j = 0; j < newHeight; j++) {
                cs225::HSLAPixel &scaledPixel = scaledImage.getPixel(i, j);
                cs225::HSLAPixel &originalPixel = getPixel(i / factor, j / factor);
                scaledPixel = originalPixel;
            }
        }
        *this = scaledImage;
    }

    /**
     * Scales the image to fit within the size (`w` x `h`).  This function preserves
     * the aspect ratio of the image, so the result will always be an image of width
     * `w` or of height `h` (not necessarily both).
     *
     * For example, if the Image is currently (10, 20) and we want to scale to
     * (100, 100), then this should scale the Image to (50, 100). Note that scaling
     * the image to (100, 200) does not fit within the (100,100) dimensions given
     * and is therefore incorrect.
     *
     * This function both resizes the Image and scales the contents.
     *
     * @param w Desired width of the scaled Image
     * @param h Desired height of the scaled Image
    **/
    void Image::scale(unsigned w, unsigned h) {
        double xFactor = (double)w / width();
        double yFactor = (double)h / height();
        double scaleFactor = std::min(xFactor, yFactor);
        scale(scaleFactor);
}