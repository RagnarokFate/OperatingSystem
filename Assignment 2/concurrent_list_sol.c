#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "concurrent_list.h"

struct node {
    int value;
    node* next;
    node* prev;

    pthread_mutex_t lock;

    //
    // add more fields
};

struct list {
    node* head;

    pthread_mutex_t lock;
    // add fields
};

void print_node(node* node)
{
    // DO NOT DELETE
    if(node)
    {
        printf("%d ", node->value);
    }
}

list* create_list()
{
    list *new_list = (list*)malloc(sizeof(list));
    new_list->head = NULL;

    return new_list;

}

void delete_list(list* list)
{

    pthread_mutex_lock(&list->lock);

    node* temp = list->head;

    while (list->head != NULL)
    {
        temp = list->head;
        list->head = list->head->next;
        free(temp);
    }

    pthread_mutex_unlock(&list->lock);
    free(list);
}

void insert_value(list* list, int value)
{

    node* new_node = (node*)malloc(sizeof(node));
    new_node->value = value;
    new_node->next = NULL;

    node* temp = list->head;

    //list is empty
    if (temp == NULL)
    {
        list->head = new_node;
        return;
    }

    //there are at least one element


    pthread_mutex_lock(&temp->lock);

    //insert before the first element
    if (temp->value >= value)
    {
        new_node->next = temp;
        list->head = new_node;
        pthread_mutex_unlock(&temp->lock);
        return;
    }


    node* temp_prev =temp;
    temp = temp->next;
    if(temp)
        pthread_mutex_lock(&temp->lock);

    node* next=NULL;
    if(temp)
        next= temp->next;



    while (temp != NULL)
    {
        if(next)
            pthread_mutex_lock(&next->lock);

        //insert after temp
        if (temp->value >= value)
        {
            new_node->next = temp;
            temp_prev->next = new_node;

            pthread_mutex_unlock(&temp->lock);
            pthread_mutex_unlock(&temp_prev->lock);

            if(next)
                pthread_mutex_unlock(&next->lock);
            return;
        }

        pthread_mutex_unlock(&temp_prev->lock);
        temp_prev = temp;
        temp = temp->next;

        if(temp)
            next = temp->next;


    }

    // the value we are inserting bigger than all insert to the last
    if (temp == NULL)
    {
        temp_prev->next = new_node;
        pthread_mutex_unlock(&temp_prev->lock);
    }

}

void remove_value(list* list, int value)
{
    if(!list)
        return;
    node* temp = list->head;


    //empty list
    if (!temp)
    {
        return;
    }

    pthread_mutex_lock(&temp->lock);

    if(temp->next)
        pthread_mutex_lock(&temp->next->lock);


    //delete the first element and update the head
    if (temp->value == value)
    {
        list->head = temp->next;

        if (temp->next)
            pthread_mutex_unlock(&temp->next->lock);

        temp->next = NULL;

        pthread_mutex_unlock(&temp->lock);
        free(temp);
        return;
    }
    else
    {
        if (temp->value > value)
        {
            if (temp->next)
                pthread_mutex_unlock(&temp->next->lock);

            pthread_mutex_unlock(&temp->lock);
            return;
        }
    }

    node* temp_prev =temp;
    temp = temp->next;


    while (temp != NULL)
    {

        // not exist
        if (temp->value > value)
        {

            pthread_mutex_unlock(&temp_prev->lock);

            pthread_mutex_unlock(&temp->lock);
            return;

        }


        if (temp->next)
            pthread_mutex_lock(&temp->next->lock);

        //delete
        if (temp->value == value)
        {
            temp_prev->next = temp->next;

            if (temp->next)
                pthread_mutex_unlock(&temp->next->lock);

            pthread_mutex_unlock(&temp->lock);

            pthread_mutex_unlock(&temp_prev->lock);

            temp->next = NULL;
            free(temp);
            return;
        }


        pthread_mutex_unlock(&temp_prev->lock);
        temp_prev = temp;
        temp = temp->next;
    }

    pthread_mutex_unlock(&temp_prev->lock);

}

void print_list(list* list)
{
    if (list)
    {
        pthread_mutex_lock(&list->lock);
        node* temp = list->head;

        while (temp != NULL)
        {
            print_node(temp);
            temp = temp->next;
        }
        pthread_mutex_unlock(&list->lock);
    }

    printf("\n"); // DO NOT DELETE
}

void print_list1(list* list)
{
    if (list)
    {
        node* temp = list->head;

        while (temp != NULL)
        {
            pthread_mutex_lock(&temp->lock);
            print_node(temp);
            pthread_mutex_unlock(&temp->lock);
            temp = temp->next;
        }
    }

    printf("\n"); // DO NOT DELETE
}

void count_list(list* list, int (*predicate)(int))
{
    if(!list)
        return;
    int count = 0; // DO NOT DELETE

    pthread_mutex_lock(&list->lock);
    node* temp = list->head;

    while (temp != NULL)
    {
        if (predicate(temp->value))
        {
            count++;
        }

        temp = temp->next;
    }
    // add code here

    printf("%d items were counted\n", count); // DO NOT DELETE

    pthread_mutex_unlock(&list->lock);
}


void count_list1(list* list, int (*predicate)(int))
{
    if(!list)
        return;
    int count = 0; // DO NOT DELETE

    node* temp = list->head;

    while (temp != NULL)
    {
        pthread_mutex_lock(&temp->lock);
        if (predicate(temp->value))
        {
            count++;
        }

        pthread_mutex_unlock(&temp->lock);
        temp = temp->next;
    }
    // add code here

    printf("%d items were counted\n", count); // DO NOT DELETE

}
