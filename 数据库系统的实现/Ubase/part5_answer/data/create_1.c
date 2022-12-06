#include <stdio.h>
#pragma pack(push)
#pragma pack(1)
int errno;

typedef struct {
  char classID[7];
  char className[20];
} Class;

typedef struct {
  char studentID[9];
  char name[16];
  char gender[4];
  int age;
  float grade;
  char classID[7];
} Student;

main()
{
  FILE *fp;
  
  Class myclass[5] = {
    { "0346321","计科031班" },
    { "0346322","计科032班" },
    { "0346324","计科034班" },
    { "yjs2004","2004级研究生" },
    { "yjs2005","2005级研究生" }
  };

  Student student[] = {
    { "034632214","贾晓丽","男",24,80.5,"0346321" },
    { "034632219","黄  源","男",21,90.0,"0346322" },
    { "034632251","田  雪","女",24,89.5,"0346324" },
    { "034632256","李兴成","男",23,88.5,"0346324" },
    { "034632260","王文婷","女",23,87.5,"0346324" },
    { "034632261","田秀钰","女",24,86.5,"0346324" },
    { "034632271","唐  征","男",23,85.5,"0346324" },
    { "034632277","唐建国","男",20,84.5,"0346324" },
    { "034632278","王  雷","男",24,83.5,"0346324" },
    { "034632279","高广智","男",23,82.5,"0346324" },
    { "034632280","袁  杰","男",24,81.5,"0346324" },
    { "y20040001","杨  挺","男",24,95.5,"yjs2004" },
    { "y20040002","司红敏","女",20,94.5,"yjs2004" },
    { "y20040003","冉慧敏","女",21,93.05,"yjs2004" },
    { "y20050001","冯向萍","女",23,92.15,"yjs2005" },
    { "y20050002","李  萍","女",34,91.12,"yjs2005" },
    { "y20050003","朱  平","男",30,90.88,"yjs2005" }
  };

  int nitems;

  /*  创建student数据文件 */
  fp = fopen("student.data","wb");
  printf("sizeof(Student):%d\n",sizeof(Student));
  nitems = fwrite( (void *)student, sizeof(Student), 17, fp);
  if ( nitems != 17 )
    fprintf(stderr, "Error in creating file\n");
  fclose(fp);

  /* create Class */
  fp = fopen("class.data","wb");
  printf("sizeof(Class):%d\n",sizeof(Class));
  nitems = fwrite( (void *)myclass, sizeof(Class), 5, fp);
  if ( nitems != 5 )
    fprintf(stderr, "Error in creating file\n");
  fclose(fp);
}
