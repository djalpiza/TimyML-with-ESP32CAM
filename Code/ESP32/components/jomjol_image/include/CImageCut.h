#include "CImageBasis.h"

class CImageCut : public CImageBasis
{
    public:
        CImageCut(int _width, int _height, int _channels) : CImageBasis(_width, _height, _channels) {ImageTMP = NULL;};
        CImageCut(std::string _image) : CImageBasis(_image) {ImageTMP = NULL;};
        CImageCut(uint8_t* _rgb_image, int _channels, int _width, int _height, int _bpp) : CImageBasis(_rgb_image, _channels, _width, _height, _bpp) {ImageTMP = NULL;};
        CImageCut(CImageBasis *_org, CImageBasis *_temp);

        int t0_dx, t0_dy, t1_dx, t1_dy;
        CImageBasis *ImageTMP;
        
        CImageBasis* CutAndSave(int x1, int y1, int dx, int dy);
        void CutAndSave(int x1, int y1, int dx, int dy, CImageBasis *_target);
};

