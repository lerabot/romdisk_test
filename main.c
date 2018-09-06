#include <kos.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <zlib/zlib.h>

extern uint8    romdisk[];
KOS_INIT_ROMDISK(romdisk);

int   mount_romdisk(char *filename, char *mountpoint){
  void  *buffer;
  char  path[100];
  int   length = 0;
  char  *dest[3];
  file_t f;

  dest[0] = "/cd";
  dest[1] = "/pc";
  dest[2] = "/sd";

  for(int i = 0; i < 3; i ++){
    sprintf(path, "%s%s", dest[i], filename);
    f = fs_open(path, O_RDONLY);
    if(f != -1) {
      length = fs_total(f);
      printf("Found romdisk at %s -> size : %u\n", dest[i], length);
      break;
    } else {
    }
  }
  fs_close(f);

  ssize_t size = fs_load(path, &buffer);
  // Successfully read romdisk image
  if(size != -1)
  {
    fs_romdisk_mount(mountpoint, buffer, 1);
    printf("Romdisk mounted at %s\n", mountpoint);
  }

  /*
  // Open file
  gzFile file = gzopen(path, "rb");
  if(!file){
    printf("No valid romdisk\n");
    return 0;
  }

  // Allocate memory, read file
  buffer = malloc(length);
  if(buffer){
    gzread(file, buffer, length);
    gzclose(file);
    // Mount
    fs_romdisk_mount(mountpoint, buffer, 1);
    printf("Romdisk mounted at %s\n", mountpoint);
    return 1;
  }
    return 0;

  */
}

int   mount_romdisk_gz(char *filename, char *mountpoint){
  void  *buffer;
  char  path[100];
  int   length = 0;
  char  *dest[3];
  file_t f;

  dest[0] = "/cd";
  dest[1] = "/pc";
  dest[2] = "/sd";

  for(int i = 0; i < 3; i ++){
    sprintf(path, "%s%s", dest[i], filename);
    f = fs_open(path, O_RDONLY);
    if(f != -1) {
      length = fs_total(f);
      printf("Found GZ Romdisk at %s -> size : %u\n", dest[i], length);
      break;
    } else {
    }
  }
  fs_close(f);

  // Open file
  gzFile file = gzopen(path, "rb");
  if(!file){
    printf("No valid GZ Romdisk\n");
    return 0;
  }

  // Allocate memory, read file
  buffer = malloc(length);
  if(buffer){
    gzread(file, buffer, length);
    gzclose(file);
    // Mount
    fs_romdisk_mount(mountpoint, buffer, 1);
    printf("GZ Romdisk mounted at %s\n", mountpoint);
    return 1;
  }
    return 0;

}

void  checkFile(char *filename, char *mountpoint) {
  char path[100];

  sprintf(path, "%s/%s", mountpoint, filename);
  file_t f = fs_open(path, O_RDONLY);
  printf("Opening %s : result %i\n", path, f);
  if(f != -1) {
    fs_close(f);
  }
}

int   main()
{
  //CHECK MAIN ROMDISK
  printf("------ ORIGINAL ROMDISK ---------\n");
  checkFile("test.png", "/rd");
  checkFile("testtest.png", "/rd");
  checkFile("this_filename_is_too_long.png", "/rd");
  checkFile("this_filename_is_too_long2.png", "/rd");

  //CHECK PC
  printf("------ PC PATH ---------\n");
  checkFile("test.png", "/pc/romdisk");
  checkFile("testtest.png", "/pc/romdisk");
  checkFile("this_filename_is_too_long.png", "/pc/romdisk");
  checkFile("this_filename_is_too_long2.png", "/pc/romdisk");

  //CHECK PC
  printf("------ ROMDISK PC PATH ---------\n");
  checkFile("test.png", "/pc/asset/rd_test");
  checkFile("testtest.png", "/pc/asset/rd_test");
  checkFile("this_filename_is_too_long.png", "/pc/asset/rd_test");
  checkFile("this_filename_is_too_long2.png", "/pc/asset/rd_test");
  checkFile("this_is_15c_.png", "/pc/asset/rd_test"); //15 char
  checkFile("this_is_16c__.png", "/pc/asset/rd_test"); //16 char

  //CHECK NEW ROMDISK
  printf("------ REGULAR ROMDISK ---------\n");
  mount_romdisk("/asset/rd_test_original.img", "/rd_2");
  checkFile("test.png", "/rd_2");
  checkFile("testtest.png", "/rd_2");
  checkFile("this_filename_is_too_long.png", "/rd_2");
  checkFile("this_filename_is_too_long2.png", "/rd_2");
  checkFile("this_is_15c_.png", "/rd_2"); //15 char
  checkFile("this_is_16c__.png", "/rd_2"); //16 char

  //CHECK GZ ROMDISK
  printf("------ GZ COMPRESSED ROMDISK ---------\n");
  mount_romdisk_gz("/asset/rd_test.img.gz", "/rd_gz");
  checkFile("test.png", "/rd_gz");
  checkFile("testtest.png", "/rd_gz");
  checkFile("this_filename_is_too_long.png", "/rd_gz");
  checkFile("this_filename_is_too_long2.png", "/rd_gz");
  checkFile("this_is_15c_.png", "/rd_2"); //15 char
  checkFile("this_is_16c__.png", "/rd_2"); //16 char

  printf("Exiting game.\n");
  return(0);
}
