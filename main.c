#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

typedef struct Equation
{
    float number;
    char operation;
    int priority;
} Equation;

typedef struct Stack
{
    int size;
    int capacity;
    Equation *collection;
} Stack;

typedef struct Node
{
    int data;
    char op;
    bool is_number;
    struct Node *next;
} Node;

typedef struct List
{
    Node *head;
} List;

typedef struct NodeInput
{
    char c;
    struct NodeInput *next;
} NodeInput;

typedef struct ListInput
{
    NodeInput *head;
} ListInput;

ListInput *listInput()
{
    ListInput *list = (ListInput *)malloc(sizeof(ListInput));
    if (list == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    list->head = NULL;
    return list;
}

void insertEndInput(ListInput *l, char x)
{
    NodeInput *new_node = (NodeInput *)malloc(sizeof(NodeInput));
    new_node->c = x;
    new_node->next = NULL;

    if (l->head == NULL)
    {
        l->head = new_node;
        return;
    }
    NodeInput *temp = l->head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = new_node;
}

bool check_if_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

Stack *createStack(int capacity)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->size = 0;
    stack->collection = (Equation *)malloc(stack->capacity * sizeof(Equation));
    return stack;
}

void push(Stack *stack, Equation item)
{
    stack->collection[stack->size++] = item;
}

Equation pop(Stack *stack)
{
    if (stack->size == 0)
    {
        Equation empty = {0};
        return empty;
    }
    return stack->collection[--stack->size];
}

bool if_empty_stack(Stack *stack)
{
    return stack->size == 0;
}

Node *node()
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
    {
        printf("\nMemory allocation failed!\n");
        exit(1);
    }
    node->data = 0;
    node->op = '\0';
    node->is_number = true;
    node->next = NULL;
    return node;
}

List *list()
{
    List *list = (List *)malloc(sizeof(List));
    if (list == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    list->head = NULL;
    return list;
}

void insertEnd(List *list, int value, char operation, bool is_num)
{
    Node *new_node = node();
    new_node->data = value;
    new_node->op = operation;
    new_node->is_number = is_num;

    if (list->head == NULL)
    {
        list->head = new_node;
        return;
    }
    Node *temp = list->head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = new_node;
}

int size(List *l)
{
    int count = 0;
    Node *curr = l->head;
    while (curr != NULL)
    {
        count++;
        curr = curr->next;
    }
    return count;
}

void printList(List *l)
{
    Node *curr = l->head;
    while (curr != NULL)
    {
        if (curr->is_number)
        {
            printf("%d ", curr->data);
        }
        else
        {
            printf("%c ", curr->op);
        }
        curr = curr->next;
    }
    printf("\n");
}

void printListInput(ListInput *l)
{
    NodeInput *curr = l->head;
    while (curr != NULL)
    {
        printf("%c", curr->c);
        curr = curr->next;
    }
    printf("\n");
}

void freeList(List *l)
{
    Node *curr = l->head;
    while (curr != NULL)
    {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(l);
}

int power(float base, float exp)
{
    int result = pow(base, exp);
    return result;
}

List *postfix_conversion(Equation equation[], int n)
{
    List *l1 = list();
    Stack *stack1 = createStack(n);

    for (int i = 0; i < n; i++)
    {
        if (equation[i].operation == 'n')
        {
            insertEnd(l1, equation[i].number, '\0', true);
        }
        else if (equation[i].operation == '(')
        {
            push(stack1, equation[i]);
        }
        else if (equation[i].operation == ')')
        {
            while (!if_empty_stack(stack1) && stack1->collection[stack1->size - 1].operation != '(')
            {
                Equation temp = pop(stack1);
                insertEnd(l1, 0, temp.operation, false);
            }
            if (!if_empty_stack(stack1))
            {
                pop(stack1);
            }
        }
        else if (check_if_operator(equation[i].operation))
        {
            while (!if_empty_stack(stack1) &&
                   stack1->collection[stack1->size - 1].operation != '(' &&
                   stack1->collection[stack1->size - 1].priority >= equation[i].priority)
            {
                Equation temp = pop(stack1);
                insertEnd(l1, 0, temp.operation, false);
            }
            push(stack1, equation[i]);
        }
    }

    while (!if_empty_stack(stack1))
    {
        Equation temp = pop(stack1);
        insertEnd(l1, 0, temp.operation, false);
    }

    free(stack1->collection);
    free(stack1);

    return l1;
}

int evaluate_postfix(List *postfix_list)
{
    int *stack = (int *)malloc(100 * sizeof(int));
    int top = -1;

    Node *curr = postfix_list->head;

    while (curr != NULL)
    {
        if (curr->is_number)
        {
            stack[++top] = curr->data;
        }
        else
        {
            if (top < 1)
            {
                printf("Error: Invalid expression\n");
                free(stack);
                return 0;
            }

            int operand2 = stack[top--];
            int operand1 = stack[top--];
            int result;

            switch (curr->op)
            {
            case '+':
                result = operand1 + operand2;
                break;
            case '-':
                result = operand1 - operand2;
                break;
            case '*':
                result = operand1 * operand2;
                break;
            case '/':
                if (operand2 == 0)
                {
                    printf("Error: Division by zero\n");
                    free(stack);
                    return 0;
                }
                result = operand1 / operand2;
                break;
            case '^':
                result = power(operand1, operand2);
                break;
            default:
                printf("Error: Unknown operator %c\n", curr->op);
                free(stack);
                return 0;
            }

            stack[++top] = result;
        }

        curr = curr->next;
    }

    if (top != 0)
    {
        printf("Error: Invalid expression\n");
        free(stack);
        return 0;
    }

    int final_result = stack[top];
    free(stack);
    return final_result;
}

ListInput *input_equation()
{
    printf("Enter the equation: ");
    ListInput *l1 = listInput();
    char c;

    while (1)
    {
        c = getchar();
        if (c == '\n')
        {
            break;
        }
        if (c == ' ')
        {
            continue;
        }
        insertEndInput(l1, c);
    }
    return l1;
}

int get_priority(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    if (op == '^')
        return 3;
    return 0;
}

int Listsize(ListInput *l)
{
    int count = 0;
    NodeInput *curr = l->head;
    while (curr != NULL)
    {
        count++;
        curr = curr->next;
    }
    return count;
}

Equation *convert_input(ListInput *l1, int *size)
{
    int capacity = Listsize(l1);
    Equation *equation = (Equation *)malloc(capacity * sizeof(Equation));
    int index = 0;

    NodeInput *curr = l1->head;
    while (curr != NULL)
    {
        if (curr->c >= '0' && curr->c <= '9')
        {
            int num = 0;
            while (curr != NULL && curr->c >= '0' && curr->c <= '9')
            {
                num = num * 10 + (curr->c - '0');
                curr = curr->next;
            }
            equation[index].number = num;
            equation[index].operation = 'n';
            equation[index].priority = 0;
            index++;
            continue;
        }
        else if (check_if_operator(curr->c))
        {
            equation[index].number = 0;
            equation[index].operation = curr->c;
            equation[index].priority = get_priority(curr->c);
            index++;
        }
        else if (curr->c == '(' || curr->c == ')')
        {
            equation[index].number = 0;
            equation[index].operation = curr->c;
            equation[index].priority = 0;
            index++;
        }

        curr = curr->next;
    }

    *size = index;
    return equation;
}

int main()
{
    ListInput *input = input_equation();

    printf("Input equation: ");
    printListInput(input);

    int eq_size;
    Equation *equation = convert_input(input, &eq_size);

    printf("\nParsed equation:\n");
    for (int i = 0; i < eq_size; i++)
    {
        if (equation[i].operation == 'n')
        {
            printf("%.0f ", equation[i].number);
        }
        else
        {
            printf("%c ", equation[i].operation);
        }
    }
    printf("\n");

    List *postfix = postfix_conversion(equation, eq_size);
    printf("\nPostfix notation: ");
    printList(postfix);

    int result = evaluate_postfix(postfix);
    printf("\nResult: %d\n", result);

    free(equation);
    freeList(postfix);

    // ajouter temps dexecution

    return 0;
}