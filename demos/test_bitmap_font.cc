//
// Created by huhua on 2023/10/5.
//
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <cassert>

void show_bitmap(const FT_Bitmap& bitmap) {
  unsigned int rows = bitmap.rows;
  unsigned int cols = bitmap.width;
  // rows: 13, cows: 13, pixel_mode: 1, num_gray: 2, pitch: 2
  printf("rows: %d, cows: %d, pixel_mode: %d, num_gray: %d, pitch: %d", rows, cols,
          bitmap.pixel_mode, bitmap.num_grays, bitmap.pitch);
  assert(bitmap.pixel_mode == FT_PIXEL_MODE_MONO);
  assert(bitmap.num_grays == 2);
//  std::cout << "rows: " << rows << ", cows: " << cols << std::endl;
//  for (int r = rows - 1; r >= 0; r--) {
//    for (int c = 0; c < cols; c++) {
//      auto value = *(bitmap.buffer + r * bitmap.pitch + c);
//      std::cout << int(value) << ' ';
////      if (value == 0) {
////        std::cout << "0";
////      } else {
////        std::cout << "1";
////      }
//    }
//    std::cout << std::endl;
//  }
}

void check(int error, const std::string &msg) {
  if (error != 0) {
    printf("failed(%d), msg: %s\n", error, msg.c_str());
    exit(-1);
  }
}

static void test_glyph_index(const FT_Face &face, int glyphIndex) {
//  gid = FT_Get_Char_Index(face, 26790);
  FT_Error error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_SBITS_ONLY);
  check(error, "FT_Load_Glyph");

  error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);
  check(error, "FT_Render_Glyph");
  FT_Bitmap bitmap = face->glyph->bitmap;
  show_bitmap( bitmap );
}

int main() {
  FT_Library library;
  FT_Init_FreeType(&library);
  FT_Face face;
  if (FT_New_Face(library, "c:/Users/huhua/Documents/simsun.ttc", 0, &face) != 0) {
    std::cout << "failed dot load face" << std::endl;
    return -1;
  }

  auto available_sizes = face->available_sizes;
  FT_Bitmap_Size *bitmap_14 = nullptr;
  int index = 0;
  for ( int i = 0; i < face->num_fixed_sizes; ++i ) {
    auto item = available_sizes + i;
    if (item->y_ppem == 64 * 14) {
      bitmap_14 = item;
      index = i;
      break;
    }
  }
  FT_Set_Pixel_Sizes(face, 14, 14);
  assert(index == 2);
  FT_Select_Size(face, index);

  int test_start_glyph_index = 1072;
  int test_end_glyph_index = 21989;
  test_glyph_index(face, 1076);

  return 0;
}