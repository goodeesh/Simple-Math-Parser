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
int count(struct node *head);
int areThereParenthesis(struct node *head);
struct node *deleteFirst(struct node *head);

int main()
{
    printf("\nInput:\n");
    char input[100];
    fgets(input, 100, stdin);
    char *input_p = input;
    // for necessary or strcom not working, makes sure the string is properly ended.
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
    struct node *head = NULL;
    head = inputToNode(input_p);
    printf("Term: ");
    printList(head);
    printf("\n");
    while (1)
    {
        printf("Input:\n");
        fgets(input, 100, stdin);
        // for necessary or strcom not working, makes sure the string is properly ended.
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
        if (strcmp(input, "=") == 0)
        {
            break;
        }
        else
            addLast(head, input_p);
        printf("Term: ");
        printList(head);
        printf("\n");
    }
    // se termina de crear el term, comienza resolver
    int position;
    int positionOpenParenthesis;
    int positionCloseParentesis;
    // PARTE CON PARENTESIS
    while (areThereParenthesis(head) == 1)
    {
        while (1)
        {
            positionOpenParenthesis = findLastOpenParenthesis(head);
            positionCloseParentesis = findClosingParenthesis(head, findLastOpenParenthesis(head));
            // sale del while, si no hay mas operators entre los parentesis y borra parentesis
            if (howManyOperators(head, positionOpenParenthesis) == 0)
            {
                if (positionOpenParenthesis == 0)
                {
                    head = deleteFirst(head);
                }
                else
                {
                    deleteFromPosition(head, positionOpenParenthesis);
                }
                positionCloseParentesis--;
                deleteFromPosition(head, positionCloseParentesis);
                break;
            }
            position = findOperator(head, findLastOpenParenthesis(head));
            operate(head, position);
            printf("Resulting term: ");
            printList(head);
            printf("\n");
        }
    }
    // PARTE SIN PARENTESIS
    while (findOperator(head, 0) != -1)
    {
        position = findOperator(head, 0); 
        if (position == 1)                // significa que se convierte en el head nuevo
        {   
            head = operate(head, position);
        }
        else
        {
            operate(head, position);
        }
        printf("Resulting term: ");
        printList(head);
        printf("\n");
    }
    free(head);
    return 0;
}

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
    return 0;
}

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

struct node *operate(struct node *head, int position)
{
    int number_one, number_two;
    char operator;
    struct node *operation = (struct node *)malloc(sizeof(struct node));
    struct node *temp = head;
    int i = 0;
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

    // necesito eliminar memoria de nodes calculados
    // este codigo vale para posicion 1, pero no para posicion
    if (temp != NULL)
    {
        operation->next = temp->next;
    }
    else
    {
        operation->next = NULL;
    }
    if (position == 1)
    {
        struct node *temp_1 = head;
        struct node *temp_2 = temp_1->next;
        struct node *temp_3 = temp_2->next;
        head->next = operation->next;
        head->number = operation->number;
        head->operator = operation->operator;
        head->type = operation->type;
        free(operation);
        free(temp_2);
        free(temp_3);
        return head;
    }
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

int findOperator(struct node *head, int position) // find next operator - edit to next operator to do
{
    int position_start = 0;
    int aux = -1;
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
            break;
        }
        if (head->type == operator_type)
        {

            if (head->operator== '*' || head->operator== '/')
            {
                aux = position_start;
                return aux;
            } else if (aux == -1)
            {
                aux = position_start;
            }
        }
        head = head->next;
        position_start++;
    }
    return aux;
}
int howManyOperators(struct node *head, int position) // find next operator - edit to next operator to do
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

void deleteFromPosition(struct node *head, int position)
{
    struct node *temp = head;
    if (position == 0)
    {
        head = head->next;
        free(temp);
    }
    else
    {
        for (int i = 0; i < position - 1; i++)
        {
            temp = temp->next;
        }
        struct node *temp2 = temp->next;
        struct node *temp3 = temp2->next;
        temp->next = temp3;
        free(temp2);
    }
}

int count(struct node *head)
{
    int count = 0;
    while (head != NULL)
    {
        head = head->next;
        count++;
    }
    count--;
    return count;
}

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

struct node *deleteFirst(struct node *head)
{
    struct node *temp = head->next;
    free(head);
    return temp;
}
