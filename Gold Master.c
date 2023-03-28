#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum node_type
{
    number_type = 1,
    operator_type,
    parenthesis_type
};
struct node
{
    char operator;
    int number;
    enum node_type type;
    struct node *next;
};
struct node *inputToNode(const char *input);
struct node *createParenthesisNode(char operator);
struct node *createOperatorNode(char operator);
struct node *createNumberNode(int number);
int printList(struct node *head);
void addLast(struct node *head, const char *input);
struct node *operate(struct node *head, int position);
int findOperator(struct node *head, int position);
int findLastOpenParenthesis(struct node *head);
int findClosingParenthesis(struct node *head, int position);
void deleteFromPosition(struct node *head, int position);
int howManyOperators(struct node *head, int position);
int areThereParenthesis(struct node *head);
struct node *deleteFirst(struct node *head);

int main()
{
    // first let´s create the term
    char input[100];
    char *input_p = input;
    struct node *head = NULL;
    printf("\n");
    while (1)
    {
        printf("Input:\n");
        fgets(input, 100, stdin);
        // fgets leaves an \n at the end, so lets move it and end the string with a \0 instead
        // if i don´t do this the strcmp function won´t work properly
        for (int i = 0; input_p[i] != '\0'; i++)
        {
            if (input_p[i] == '\n')
            {
                for (int j = i; input_p[j] != '\0'; j++)
                {
                    input_p[j] = input_p[j + 1];
                }
                i--;
            }
        }
        // if the input is =, we get out immediately
        // if i put this as a condition for while i would try to create a node with a = and there is no case for that in the functions
        if (strcmp(input, "=") == 0)
        {
            break;
        }
        if (head == NULL)
        {
            head = inputToNode(input_p);
        }
        else
        {
            addLast(head, input_p);
        }
        printf("Term: ");
        printList(head);
    }
    // Term created
    int position;
    int positionOpenParenthesis;
    int positionCloseParentesis;
    // Parenthesis first
    // we will do this while as long as we find parenthesis in the list
    while (areThereParenthesis(head) == 1)
    {
        while (1)
        {
            positionOpenParenthesis = findLastOpenParenthesis(head);
            positionCloseParentesis = findClosingParenthesis(head, findLastOpenParenthesis(head));
            // sale del while, si no hay mas operators entre los parentesis y borra parentesis
            if (howManyOperators(head, positionOpenParenthesis) == 0)
            { // if the position of the first parenthesis is one we need to update the head of the list
                if (positionOpenParenthesis == 0)
                {
                    head = deleteFirst(head);
                }
                // otherwise we are just going the parenthesis from the given position
                else
                {
                    deleteFromPosition(head, positionOpenParenthesis);
                }
                // as one parenthesis was deleted, the second position changued
                positionCloseParentesis--;
                // delete second parenthesis
                deleteFromPosition(head, positionCloseParentesis);
                break;
            }
            // we get the position of the next operation to do
            position = findOperator(head, findLastOpenParenthesis(head));
            // execute the operation and print new list
            operate(head, position);
            printf("Resulting term: ");
            printList(head);
        }
    }
// Now we will do the part without parenthesis
// This while will work as long there are operators in the list
    while (findOperator(head, 0) != -1)
    {
        position = findOperator(head, 0);
        if (position == 1) // if the operator is at this position the result will be the new head
        {
            head = operate(head, position);
        }
        else
        {
            operate(head, position);
        }
        printf("Resulting term: ");
        printList(head);
    }
    free(head);
    return 0;
}
//Detect the type of input and call the function for it
// in those functions the memory for the new node and parameters will be stablished
struct node *inputToNode(const char *input)
{
    int number = atoi(input);
    if (number != 0)
    {
        return createNumberNode(number);
    }
    if (input[0] == '0')
    {
        return createNumberNode(0);
    }
    if (strcmp(input, "(") == 0 ||
        strcmp(input, ")") == 0)
    {
        return createParenthesisNode(input[0]);
    }

    if (strcmp(input, "+") == 0 ||
        strcmp(input, "-") == 0 ||
        strcmp(input, "*") == 0 ||
        strcmp(input, "/") == 0)
    {
        return createOperatorNode(input[0]);
    }
    return NULL;
}
//function for creating the new nodes
struct node *createNumberNode(int number)
{
    struct node *number_n = (struct node *)malloc(sizeof(struct node));
    number_n->number = number;
    number_n->type = number_type;
    number_n->next = NULL;
    return number_n;
}

struct node *createOperatorNode(char operator)
{
    struct node *operator_n = (struct node *)malloc(sizeof(struct node));
    operator_n->operator= operator;
    operator_n->type = operator_type;
    operator_n->next = NULL;
    return operator_n;
}

struct node *createParenthesisNode(char operator)
{
    struct node *parenthesis_n = (struct node *)malloc(sizeof(struct node));
    parenthesis_n->operator= operator;
    parenthesis_n->type = parenthesis_type;
    parenthesis_n->next = NULL;
    return parenthesis_n;
}
// here we print the list
int printList(struct node *head)
{
    while (head != NULL)
    {
        if (head->type == number_type)
        {
            printf("%d", head->number);
            head = head->next;
        }
        else if (head->type == operator_type || head->type == parenthesis_type)
        {
            printf("%c", head->operator);
            head = head->next;
        }
    }
    printf("\n");
    return 0;
}
//function to add the new nodes at the end of the list
void addLast(struct node *head, const char *input)
{
    struct node *newLast = inputToNode(input);
    while (head->next != NULL)
    {
        head = head->next;
    }
    head->next = newLast;
    newLast->next = NULL;
}  
//the function to do the operations, where new memory will be assign and old will be set free
struct node *operate(struct node *head, int position)
{
    int number_one, number_two;
    char operator;
    struct node *operation = (struct node *)malloc(sizeof(struct node)); //get new memory for the new node
    struct node *temp = head; // saving the adress of head to travel between to the nodes
    int i = 0;
    //creating the new node
    while (i < position + 2) 
    {
        if (i == (position - 1))
        {
            number_one = temp->number;
        }
        if (i == (position))
        {
            operator= temp->operator;
        }
        if (i == (position + 1))
        {
            number_two = temp->number;
            break;
        }
        temp = temp->next;
        i++;
    }
    //setting a variable for a switch, depending on the operator we will do one operation or the other
    int op;
    if (operator== '+')
        op = 1;
    else if (operator== '-')
        op = 2;
    else if (operator== '*')
        op = 3;
    else if (operator== '/')
        op = 4;

    switch (op)
    {
    case 1:
        operation->number = number_one + number_two;
        break;
    case 2:
        operation->number = number_one - number_two;
        break;
    case 3:
        operation->number = number_one * number_two;
        break;
    case 4:
    //in case of division with 0, we use 1 instead
        if (number_two == 0)
        {
            number_two = 1;
        }
        operation->number = number_one / number_two;
        break;
    default:
        break;
    }
    operation->type = number_type;
    operation->next = temp->next;
    //new node is completed
    //when position is 1, the result will be the new head
    //we update the information from head and set free all the memory we no longer need, including the operation itself
    if (position == 1)
    {
        struct node *temp_1 = head;
        struct node *temp_2 = temp_1->next;
        struct node *temp_3 = temp_2->next;
        head->next = operation->next;
        head->number = operation->number;
        head->operator= operation->operator;
        head->type = operation->type;
        free(operation);
        free(temp_2);
        free(temp_3);
        return head;
    }
    //we take the head until the position before the first number we are going to delete
    //we set the new next to the node with the result
    //list is updated now, so we set free the memory no longer needed
    else
    {
        for (int i = 0; i < position - 2; i++)
        {
            head = head->next;
        }
        struct node *temp_1 = head->next;
        struct node *temp_2 = temp_1->next;
        struct node *temp_3 = temp_2->next;
        free(temp_1);
        free(temp_2);
        free(temp_3);
        head->next = operation;
        return operation;
    }
}
//we find the next operator to work with
int findOperator(struct node *head, int position) 
{
    int position_start = 0; //used in case of a parenthesis
    int aux = -1;
    while (head != NULL)
    {
        if (position_start < position) //we move the head until we are at the desired start position
        {
            position_start++;
            head = head->next;
            continue; // using continue we skip the other code until we are at the right position
        }
        else if (head->operator== ')') // if there is a ) it means we are in a parenthesis, otherwise it would be deleted
        {
            break; 
        }
        if (head->type == operator_type)
        {

            if (head->operator== '*' || head->operator== '/') //as soon as we find one we return the position
            {
                aux = position_start;
                return aux;
            }
            else if (aux == -1) //aux -1 means it´s the first parameter, if we already have a valid position, we keep it
            {
                aux = position_start; // no break, just in case a / or * come later
            }
        }
        head = head->next; 
        position_start++;
    }
    return aux;
}
//used to know how many operators there are inside two parenthesis, similar to find operator in the start position logic
int howManyOperators(struct node *head, int position)
{
    int position_start = 0;
    int aux = 0;
    while (head != NULL)
    {
        if (position_start < position)
        {
            position_start++;
            head = head->next;
            continue;
        }
        else if (head->type == operator_type)
        {
            aux++;
        }
        head = head->next;
        position_start++;
        if (head->operator== ')')
        {
            break;
        }
    }
    return aux;
}
//gives position of last (, because it has priority as it is inside another one
int findLastOpenParenthesis(struct node *head)
{
    int position = 0;
    int aux = -1;
    while (head != NULL)
    {
        if (head->operator== '(')
        {
            aux = position;
            head = head->next;
        }
        else
        {
            head = head->next;
        }
        position++;
    }
    return aux;
}
//gives position of ), using start position logic again
int findClosingParenthesis(struct node *head, int position)
{
    int position_start = 0;
    int aux = 0;
    while (head != NULL)
    {
        if (position_start < position)
        {
            position_start++;
            head = head->next;
            continue;
        }
        else if (head->operator== ')')
        {
            aux = position_start;
            return aux;
        }
        else
        {
            head = head->next;
        }
        position_start++;
    }
    return aux;
}
//name says it all
void deleteFromPosition(struct node *head, int position)
{
    struct node *temp = head; 
    if (position == 0) // head will be updated in case of position being 0
    {
        head = head->next;
        free(temp);
    }
    else
    {
        for (int i = 0; i < position - 1; i++) //otherwise we go just before the postion
        {
            temp = temp->next;
        }
        struct node *temp2 = temp->next; // save the info of the next
        struct node *temp3 = temp2->next; // save the info of the new next
        temp->next = temp3; //set the new next to the position before the item to delete
        free(temp2); // delete item at desire position
    }
}
//name says it all
int areThereParenthesis(struct node *head)
{
    while (head != NULL)
    {
        if (head->type == parenthesis_type)
        {
            return 1;
        }
        head = head->next;
    }
    return 0;
}
//used when the parenthesis is at the beggining, the parenthesis is deleted and the result from the operation becomes the new head
struct node *deleteFirst(struct node *head)
{
    struct node *temp = head->next;
    free(head);
    return temp;
}
