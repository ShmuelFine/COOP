# COOP CV Modules

The `CV` directory contains a self-contained, COOP-styled image-processing pipeline that mirrors a subset of the classic Computer Vision toolbox.

> ### **Configuration Required for CV Unit Tests**
>
> To run the `GrayImageUnitTest` successfully, you must first update a hardcoded file path inside the test file.
>
> 1.  Open `CV/UnitTest/GrayImageUnitTest.c`.
> 2.  At the top of the file, find the `#define PATH` macro.
> 3.  Change the default path to the **absolute path** of the COOP project name directory on your local PC.
>
> **Example:**
> ```c
> // The path should be the path of the folder where your image is located.
> #define PATH  "C:\\MyProjects\\COOP-Project\\"
> ```
>
> The tests use this `PATH` macro to locate and load the sample images (like `tiger_input.bmp`) from the `UnitTest/Images/` folder. If this is not updated, the tests will fail to load the images.

- **`CV/CV`** classes:
  - `GrayImage` wraps grayscale rasters with safe constructors, bounds-checked pixel access, and automatic memory ownership.
  - `GaussianProcessor`, `SobelProcessor`, `BorderProcessor`, `ThresholdProcessor`, and `NMSProcessor` model each stage of an edge-detection pipeline as composable classes with explicit lifetimes.
  - `ImageProcessor` orchestrates multi-step operations.
- **`CV/UnitTest`** provides test runners and fixtures (`GrayImageUnitTest.c`, `_RUNNER_.c`) that validate each stage on sample BMP frames stored under `UnitTest/Images/`

### GrayImage Class API

The `GrayImage` class is the core component for image handling. It provides the following primary interfaces for construction, management, and processing:

#### Constructors and Lifetime Methods

* **`DEF_CTOR(GrayImage)`**: The default constructor. It initializes an empty `GrayImage` object, setting its dimensions and data pointers to zero.
* **`init(width, height, data_vector)`**: Allocates a new buffer of the specified dimensions. If `data_vector` is provided, its contents are copied into the new buffer; otherwise, the buffer is zero-initialized.
* **`clone(out_clone)`**: Performs a **deep copy**. It creates a new, independent `GrayImage` in `out_clone` with its own data buffer containing a copy of the source image's pixels.
* **`init_copy(other)`**: Performs a **shallow copy**. The new image shares the *same* internal data buffer as the `other` image. The buffer's reference count is incremented.
* **`init_move(other)`**: Moves ownership of the image buffer from `other` to the new image. The `other` image is reset to an empty state, and its reference count is transferred.
* **`init_ROI(other, row, col, width, height)`**: Creates a **Region of Interest (ROI)**. This is a shallow copy that points to the `other` image's buffer but adjusts its internal offsets to represent only a sub-rectangle of the original image.
* **`DEF_DTOR(GrayImage)`**: The destructor. It implements reference counting; the internal `image_buffer` is only freed when the reference count drops to zero, allowing multiple `GrayImage` objects to safely share the same underlying image data.

#### Operators

* **`add(other, out)`**: Performs pixel-wise addition (`this` + `other`), clamping the result at 255 (saturation).
* **`sub_default(other, out)`**: Performs pixel-wise subtraction (`this` - `other`), clamping the result at 0 (negative values become 0).
* **`sub_abs(other, out)`**: Calculates the absolute pixel-wise difference |`this` - `other`|.
* **`mul_scalar(alpha, out)`**: Multiplies every pixel value by the scalar `alpha`, clamping the result between 0-255.
* **`mul_mat(other, out)`**: Performs standard matrix multiplication (`this` * `other`). Assumes `this->width` equals `other->height`.
* **`equals(other, out_comparison_image)`**: Performs a pixel-wise comparison. The output image is filled with 255 for pixels that are identical in both images and 0 for pixels that differ.

#### Main Methods

* **`get_width() / get_height() / get_stride()`**: A set of simple accessor methods ("getters") that return the image's properties (width, height, and stride).
* **`get_pixel_ptr(MEM_SIZE_T x, MEM_SIZE_T y, uint8_t** out_ptr)`**: Provides direct, raw pointer access to a specific pixel at coordinates (x, y) for efficient read/write operations.
* **`save_to_bmp(filePath)`**: Saves the `GrayImage` instance to disk as an 8-bit grayscale BMP file at the specified path.
* **`load_from_bmp(path)`**: Loads an image from the specified BMP file. It supports 8-bit grayscale and 24-bit color BMPs (which are automatically converted to 8-bit grayscale upon loading).
* **`canny(float low_thresh, float high_thresh)`**: This is the main processing function. It executes the complete Canny edge detection pipeline on the image instance itself by creating and running a series of processors (`GaussianProcessor`, `SobelProcessor`, `NMSProcessor`, `ThresholdProcessor`, and `BorderProcessor`).

<table>
  <tr>
    <td style="text-align: center;">
      <em>Original image</em>
    </td>
    <td style="text-align: center;">
         <em> Image after canny() processing</em>
    </td>
  </tr>
    <tr>
    <td style="text-align: center;">
      <img src="tiger_input.bmp"  width="300">
    </td>
    <td style="text-align: center;">
      <img src="canny_output.bmp"  width="300">    
    </td>
  </tr>
</table>


