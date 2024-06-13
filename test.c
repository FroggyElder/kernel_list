#include "kernel_list.h"
#include <stdio.h>
#include <string.h>

struct student {
    struct list_head head;
    char* name;
    int age;
};

static const unsigned long student_offset = ((unsigned long)(&(((struct student*)0)->head)));
#define STUDENT_FROM_HEAD(head) ((struct student*)((void*)head-student_offset))

struct student* createStudent (char* name,int age) {
    //make space
    struct student* new_student = (struct student*)malloc(sizeof(struct student));
    if(new_student==NULL) {
        printf("%s:",__func__);
        perror("");
        goto init_error;
    }

    //fill the data
    if(name!=NULL) {
        new_student->name = (char*)malloc(sizeof(char)*strlen(name)+1);
        strcpy (new_student->name,name);
        new_student->age = age;
    }
    else {
        new_student->name = NULL;
        new_student->age = -1;
    }

    //if all is good
    INIT_LIST_HEAD(&new_student->head);
    return new_student;

init_error:
    return NULL;
}

bool killStudent (struct student* stu) {
    if (stu==NULL) return false;

    list_del(&stu->head);

    if (stu->name!=NULL) free(stu->name);

    free(stu);
    return true;
}

bool killStudents (struct student* stu) {
    if (stu==false) {
        return false;
    }

    while (stu->head.next != &stu->head)
        killStudent(STUDENT_FROM_HEAD(stu->head.next));

    return true;
}

int main () {
    struct student* list0 = createStudent(NULL,0);
    struct list_head* p = &list0->head;


    for (int i=1;i<=1024;i=i<<1) {
        char name[32];
        sprintf(name,"clone%d",i);
        struct student* clone = createStudent(name,i);
        list_add(&clone->head,p);
        p=p->next;
    }

    list_for_each(p,&list0->head)
        printf("[%s:%d] ",STUDENT_FROM_HEAD(p)->name,STUDENT_FROM_HEAD(p)->age);

    printf("\n");

    // list_for_each(p,&list0->head)
    //     if(STUDENT_FROM_HEAD(p)->age==256) {
    //         killStudent(STUDENT_FROM_HEAD(p));
    //         break;
    //     }
    
    struct student* st_p = NULL;
    list_for_each_entry(st_p,&list0->head,head) {
        if(st_p->age==256) {
            killStudent(st_p);
            break;
        }
    }

    list_for_each(p,&list0->head)
        printf("[%s:%d] ",STUDENT_FROM_HEAD(p)->name,STUDENT_FROM_HEAD(p)->age);

    printf("\n");

    killStudents(list0);
}