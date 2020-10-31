/*
Group Number 64
Chirag C D - 2018A7PS0277P
R Rohit - 2018A7PS0224P
Abhishek Agarwal - 2018A7PS0177P
Akarsh Gowtham Shroff - 2018A7PS1121P
*/

#include "parser.h"

link * copy_link(link * source) {
    link * new = (link *) malloc(sizeof(link));
    new->arr_info = source->arr_info;
    new->arr_storage = source->arr_storage;
    new->type = source->type;
   return new;
}

void add_info(parseTree * node, link * info, typeExpressionTable * table) {
    strcpy(info->id, node->children[0]->term.type.tok.lexeme);
    put_link(table, info);
    if(node->num_children > 1) add_info(node->children[1], copy_link(info), table);
}

int get_length_idx(parseTree * idx) {
    if(idx->num_children == 1) return 0;
    return 1 + get_length_idx(idx->children[1]);
}

int get_length_idx_2(parseTree * idx) {
    if(idx->num_children == 1) return 0;
    return 1 + get_length_idx_2(idx->children[2]);
}

link * run_primitive(parseTree * node) {
    link * info = (link *) malloc(sizeof(link));
    info->arr_info = PRIMITIVE;
    info->arr_storage = NONE;
    // printf("Is Bool %d\n", node->children[0]->term.type.tok.token == KEY_BOOL);
    if(node->children[0]->term.type.tok.token == KEY_INT) info->type.prim_info = INTEGER;
    else if(node->children[0]->term.type.tok.token == KEY_BOOL) info->type.prim_info = BOOLEAN;
    else if(node->children[0]->term.type.tok.token == KEY_REAL) info->type.prim_info = REAL;
    else printf("line : %2d | stmt : declaration | message : Unknown primitive %s\n", node->children[0]->term.type.tok.line_num, node->children[0]->term.type.tok.lexeme);
    return info;
}

link * run_jagged(parseTree * dec, parseTree * init) {
    link * info = (link *) malloc(sizeof(link));
    info->arr_info = JAG_ARR;
    info->arr_storage = NONE;
    info->type.jagged_arr_info.betype = INTEGER;
    info->type.jagged_arr_info.num_dim = (dec->term.type.nt == JAGGED2_DEC) ? 2 : 3;
    int dim2 = (info->type.jagged_arr_info.num_dim == 2);
    parseTree * x_node = dec->children[3]->children[0]->children[0];
    parseTree * y_node = dec->children[3]->children[2]->children[0];

    if(x_node->term.type.tok.token == ID || y_node->term.type.tok.token == ID) {
        printf("line : %2d | stmt : declaration | message : Jagged arrays cannot have dynamic bounds\n", x_node->term.type.tok.line_num);
        return NULL;
    }
    info->type.jagged_arr_info.range_R1.r1 = atoi(x_node->term.type.tok.lexeme);
    info->type.jagged_arr_info.range_R1.r2 = atoi(y_node->term.type.tok.lexeme);
    if(info->type.jagged_arr_info.range_R1.r1 > info->type.jagged_arr_info.range_R1.r2) {
        printf("line : %2d | stmt : declaration | message : Lower bound cannot exceed upper bound\n", x_node->term.type.tok.line_num);
        return NULL;
    }

    int range_1 = 1 + info->type.jagged_arr_info.range_R1.r2 - info->type.jagged_arr_info.range_R1.r1;
    info->type.jagged_arr_info.range_R2 = (rng_R2 *) malloc(range_1 * sizeof(rng_R2));
    
    if(dim2) {
        for(int i = 0; i < range_1; i++) {
            info->type.jagged_arr_info.range_R2[i].num_dim = 1;
            info->type.jagged_arr_info.range_R2[i].dims = (int *) malloc(sizeof(int));
        }
        int count = 0;
        for(int i = 0; i < range_1; i++) {
            if((i < range_1 - 1) && init->num_children == 1 || (i == range_1 - 1) && init->num_children > 1) {
                printf("line : %2d | stmt : declaration | message : R1 Size mismatch\n", init->children[0]->children[0]->term.type.tok.line_num);
                return NULL;
            }
            parseTree * single_init = init->children[0];
            info->type.jagged_arr_info.range_R2[i].dims[0] = atoi(single_init->children[6]->term.type.tok.lexeme);
            int act_length = 1 + get_length_idx_2(single_init->children[10]->children[1]);
            if(act_length != info->type.jagged_arr_info.range_R2[i].dims[0]) {
                printf("line : %2d | stmt : declaration | message : R2 Size mismatch\n", init->children[0]->children[0]->term.type.tok.line_num);
                return NULL;
            }
            if(i < range_1 - 1) init = init->children[1];
        }
        return info;
    }
    
    for(int i = 0; i < range_1; i++) {
        int num_dim = atoi(init->children[0]->children[6]->term.type.tok.lexeme);
        info->type.jagged_arr_info.range_R2[i].num_dim = num_dim;
        info->type.jagged_arr_info.range_R2[i].dims = (int *) malloc(num_dim * sizeof(int));
        parseTree * range = init->children[0]->children[10];

        for(int j = 0; j < num_dim; j++) {
            int length = get_length_idx(range->children[0]);
            if(length == 0) {
                printf("line : %2d | stmt : declaration | message : Detected 0 length subrange\n", init->children[0]->children[6]->term.type.tok.line_num);
                return NULL;
            }
            info->type.jagged_arr_info.range_R2[i].dims[j] = length;
            if(range->num_children < 3 && (j < num_dim - 1) || range->num_children == 3 && (j == num_dim - 1)) {
                printf("line : %2d | stmt : declaration | message : R2 Size mismatch\n", range->children[0]->children[0]->term.type.tok.line_num);
                return NULL;
            }
            else if(j < num_dim-1) range = range->children[2];
        }
        if(init->num_children < 2 && (i < range_1 - 1) || init->num_children == 2 && (i == range_1 - 1)) {
            printf("line : %2d | stmt : declaration | message : R1 Size mismatch\n", init->children[0]->children[6]->term.type.tok.line_num);
            return NULL;
        }
        if(i < range_1 - 1) init = init->children[1];
    }
    return info;
}

int get_depth(parseTree * node) {
    if(node->num_children < 4) return 1;
    else return 1 + get_depth(node->children[3]);
}

int fill_ranges(parseTree * node, Var_Pair * pairs, typeExpressionTable * table, enum arr_storage * store_type) {
    parseTree * lower = node->children[1]->children[0]->children[0];
    parseTree * higher = node->children[1]->children[2]->children[0];
    if(lower->term.type.tok.token == ID) {
        link * bound = get_link(table, lower->term.type.tok.lexeme);
        if(!bound) {
            printf("line : %2d | stmt : declaration | message : Identifier not declared\n", lower->term.type.tok.line_num);
            return -1;
        }
        if(!(bound->arr_info == PRIMITIVE && bound->type.prim_info == INTEGER)) {
            printf("line : %2d | stmt : declaration | message : Non integer bounds not allowed\n", lower->term.type.tok.line_num);
            return -1;
        }
        *store_type = DYNAMIC;
        pairs[0].is_r1_static = 0;
        strcpy(pairs[0].r1.r_d, lower->term.type.tok.lexeme);
    }
    else {
        pairs[0].is_r1_static = 1;
        pairs[0].r1.r_s = atoi(lower->term.type.tok.lexeme);
    }
    if(higher->term.type.tok.token == ID) {
        link * bound = get_link(table, higher->term.type.tok.lexeme);
        if(!bound) {
            printf("line : %2d | stmt : declaration | message : Identifier not declared\n", higher->term.type.tok.line_num);
            return -1;
        }
        if(!(bound->arr_info == PRIMITIVE && bound->type.prim_info == INTEGER)) {
            printf("line : %2d | stmt : declaration | message : Non integer bounds not allowed\n", higher->term.type.tok.line_num);
            return -1;
        }
        *store_type = DYNAMIC;
        pairs[0].is_r2_static = 0;
        strcpy(pairs[0].r2.r_d, higher->term.type.tok.lexeme);
    }
    else {
        pairs[0].is_r2_static = 1;
        pairs[0].r2.r_s = atoi(higher->term.type.tok.lexeme);
    }
    if(pairs[0].is_r1_static && pairs[0].is_r2_static && (pairs[0].r1.r_s > pairs[0].r2.r_s)) {
        printf("line : %2d | stmt : declaration | message : Lower bound cannot exceed upper bound\n", higher->term.type.tok.line_num);
        return -1;
    }
    if(node->num_children == 3) return 0;
    return fill_ranges(node->children[3], pairs+1, table, store_type);
}

link * run_rect(parseTree * node, typeExpressionTable * table) {
    link * info = (link *) malloc(sizeof(link));
    info->arr_info = RECT_ARR;
    info->type.rect_arr_info.betype = INTEGER;
    int depth = get_depth(node->children[1]);
    info->type.rect_arr_info.num_dim = depth;
    info->type.rect_arr_info.dim_range = (Var_Pair *) malloc(depth * sizeof(Var_Pair));
    info->arr_storage = STATIC;
    int success = fill_ranges(node->children[1], info->type.rect_arr_info.dim_range, table, &(info->arr_storage));
    if(success == -1) return NULL;
    return info;
}

link * get_type(parseTree * tree, int is_single, typeExpressionTable * table) {
    int dec = (is_single) ? 3 : 6;
    if(tree->children[dec]->children[0]->term.is_term == 0) return run_primitive(tree->children[dec]->children[0]);
    else if(tree->children[dec]->children[0]->term.type.tok.token == KEY_ARR) return run_rect(tree->children[dec], table);
    else return run_jagged(tree->children[dec], tree->children[dec+2]);
}

void traverseDeclares(parseTree * tree, typeExpressionTable * table) {

    if(!(tree->term.is_term == 0 && tree->term.type.nt == DECL_STMT)) {
        for(int i = 0; i < tree->num_children; i++) traverseDeclares(tree->children[i], table);
        return;
    }
    int single = (tree->children[0]->term.type.nt == SINGLEVAR_DEC);
    tree = tree->children[0];

    link * info = get_type(tree, single, table);
    if(info == NULL) return;
    if(single) {
        strcpy(info->id, tree->children[1]->term.type.tok.lexeme);
        put_link(table, info);
    }
    else add_info(tree->children[4], info, table);
}


/****      ASSIGNMENT TRAVERSAL   ********/

int is_op_compatible(link l1, link l2, Token tok, ErrInfo* ei)
{
    if(l1.arr_info==l2.arr_info)
    {
        if(l1.arr_info==PRIMITIVE)
        {
            if(l1.type.prim_info!=l2.type.prim_info)
            {
                snprintf(ei->msg, 31, "primitive type mismatch");
                return 0;
            }

            if(l1.type.prim_info==INTEGER || l1.type.prim_info==REAL)
            {
                if(tok == OP_OR || tok == OP_AND)
                {
                    snprintf(ei->msg, 31, "primitive type mismatch");
                    return 0;
                }
            }

            return 1;
        }
        if(l1.arr_info==RECT_ARR)
        {
            rect_arr_te r1 = l1.type.rect_arr_info;
            rect_arr_te r2 = l2.type.rect_arr_info;

            if(r1.betype!=INTEGER || r2.betype!=INTEGER)
            {
                snprintf(ei->msg, 31, "invalid array type");
                return 0;
            }

            if(tok==OP_AND || tok==OP_OR || tok==OP_SLASH)
            {
                snprintf(ei->msg, 31, "invalid array operation");
                return 0;
            }

            if(r1.num_dim!=r2.num_dim)
            {
                snprintf(ei->msg, 31, "RA size mismatch");
            }

            Var_Pair* v1 = r1.dim_range;
            Var_Pair* v2 = r2.dim_range;

            for(int i=0;i<r1.num_dim;i++)
            {
                if(v1[i].is_r1_static==0 || v1[i].is_r2_static==0 || v2[i].is_r1_static==0 || v2[i].is_r2_static==0) 
                {
                    snprintf(ei->msg, 31, "invalid array dim.");
                    return 0;
                }
                if(v1[i].r2.r_s - v1[i].r1.r_s != v2[i].r2.r_s - v2[i].r1.r_s)
                {
                    snprintf(ei->msg, 31, "RA size mismatch");
                    return 0;
                }
            }
            return 1;
        }

        if(l1.arr_info==JAG_ARR)
        {
            jagged_arr_te r1 = l1.type.jagged_arr_info;
            jagged_arr_te r2 = l2.type.jagged_arr_info;

            if(r1.betype!=INTEGER || r2.betype!=INTEGER)
            {
                snprintf(ei->msg, 31, "invalid array type");
                return 0;
            }

            if(tok==OP_AND || tok==OP_OR || tok==OP_SLASH)
            {
                snprintf(ei->msg, 31, "invalid array operation");
                return 0;
            }

            if(r1.num_dim!=r2.num_dim) // checks for 2D and 3D jagged array
            {
                snprintf(ei->msg, 31, "JA size mismatch");
                return 0;
            }
            if(r1.range_R1.r2 - r1.range_R1.r1 != r2.range_R1.r2 - r2.range_R1.r1) // checks for same number of R1 dimensions
            {
                snprintf(ei->msg, 31, "JA size mismatch");
                return 0;
            }

            rng_R2* v1 = r1.range_R2;
            rng_R2* v2 = r2.range_R2;

            for(int i=0;i<r1.range_R1.r2-r1.range_R1.r1 + 1;i++)
            {
                if(v1[i].num_dim != v2[i].num_dim) 
                {
                    snprintf(ei->msg, 31, "JA size mismatch");
                    return 0;
                }
                for(int j=0;j<v1[i].num_dim;j++)
                {
                    if(v1[i].dims[j] != v2[i].dims[j])
                    {
                        snprintf(ei->msg, 31, "JA size mismatch");
                        return 0;
                    }
                }
            }
            return 1;
        }
    }

    snprintf(ei->msg, 31, "invalid operand types");
    return 0;
}

link get_data_type_of_id(parseTree * tree, typeExpressionTable * table, ErrInfo* ei) // tree points to parse tree node with token '<ID>'
{
    char* lexeme = tree->term.type.tok.lexeme;
    ei->line_number = tree->term.type.tok.line_num;

    link* l = get_link(table,lexeme);
    if(!l)
    {
        strcpy(ei->lex1,lexeme);
        snprintf(ei->msg, 31, "undeclared variable");
        ei->success=0;
        return (link){};     
    }
    tree->type_info = *l;
    return *l;
}

link get_data_type_var(parseTree * tree, typeExpressionTable * table, ErrInfo* ei)
{

    link l1 = get_data_type_of_id(tree->children[0],table, ei);
    if(!ei->success) return (link){};
  
    strcpy(ei->lex1,tree->children[0]->term.type.tok.lexeme);
    get_str(tree->children[0]->term.type,ei->type1,tree->children[0]->term.is_term);

    if(tree->num_children==2 && l1.arr_info==PRIMITIVE)
    {
        snprintf(ei->msg, 31, "can't index primitive");
        parseTree* tmp = tree->children[1]->children[1]->children[0]->children[0];
        strcpy(ei->lex2,tmp->term.type.tok.lexeme);
        get_str(tmp->term.type,ei->type2,tmp->term.is_term);
        ei->success=0;
        return (link){};
    }

    if(tree->num_children==2)
    {
        parseTree *r = tree->children[1];   // 'r' is pointing to parse tree node whose token is '<array_idx>'
        int curr_idx=0;
        int size=1;
        int* dims = malloc(1 * sizeof(int));

        parseTree* array_sel = r->children[1];

        int orig_depth = ei->depth;
        ei->depth += 2;
        while(1)
        {
            parseTree* tmp = array_sel->children[0]->children[0]; // 'tmp' is pointing to parse tree node whose token is '<array_sel>'
            ei->depth += 2;
            if(tmp->term.type.tok.token==ID)
            {
                snprintf(ei->msg, 31, "array dim. check failed");
                strcpy(ei->lex2,tmp->term.type.tok.lexeme);
                get_str(tmp->term.type,ei->type2,tmp->term.is_term);
                ei->success=0;
                return (link){};
            }

            if(curr_idx+1>size) dims = realloc(dims, size*=2);
            
            dims[curr_idx++] = atoi(tmp->term.type.tok.lexeme);

            if(array_sel->num_children==2) array_sel = array_sel->children[1], ei->depth -=1;
            else break;
        }

        dims = realloc(dims,curr_idx);

        ei->depth = orig_depth;
        if(l1.arr_info==RECT_ARR)
        {
            Var_Pair* found_dims = l1.type.rect_arr_info.dim_range;    // this and next 2 values obtained from type expression of variable
            int found_num_dims = l1.type.rect_arr_info.num_dim;

            if(curr_idx != found_num_dims)
            {
                snprintf(ei->msg, 31, "RA size mismatch");
                ei->success=0;
                return (link){};
            }

            for(int i=0;i<curr_idx;i++)
            {
                if(found_dims[i].is_r1_static==0 || found_dims[i].is_r2_static==0) 
                {
                    snprintf(ei->msg, 31, "RA dim. check failed");
                    ei->success=0;
                    return (link){};
                }

                if(!(dims[i]>=found_dims[i].r1.r_s && dims[i] <= found_dims[i].r2.r_s))
                {
                    snprintf(ei->msg, 31, "RA dim. out of bounds");
                    ei->success=0;
                    return (link){};
                }
            }

            l1.arr_info = PRIMITIVE;
            l1.type.prim_info = l1.type.rect_arr_info.betype;   // after indexing array element, it is a primitive integer

        }

        if(l1.arr_info==JAG_ARR)
        {
            Int_Pair found_dims_R1 = l1.type.jagged_arr_info.range_R1;    // this and next 3 values obtained from type expression of variable
            rng_R2* found_dims_R2 = l1.type.jagged_arr_info.range_R2; 
            int found_num_dims = l1.type.jagged_arr_info.num_dim;

            if(curr_idx != found_num_dims)
            {
                snprintf(ei->msg, 31, "JA dim. mismatch");
                ei->success=0;
                return (link){};
            }

            if(!(dims[0]>=found_dims_R1.r1 || dims[0] <= found_dims_R1.r2)) 
            {
                snprintf(ei->msg, 31, "JA dim. out of bounds");
                ei->success=0;
                return (link){};
            }

            if(curr_idx==2)  // if jagged array has dimension 2
            {
                    int idx = dims[0]-found_dims_R1.r1;
                    int a = dims[1]>=0 && dims[1]<found_dims_R2[idx].dims[0];
                    if (!a)
                    {
                        snprintf(ei->msg, 31, "2D JA dim. out of bounds");
                        ei->success=0;
                        return (link){};
                    }
            }
            

            if(curr_idx==3)  // if jagged array has dimension 3
            {
                int id1 = dims[0] - found_dims_R1.r1;
                rng_R2 req = found_dims_R2[id1];
                if(dims[1]>=req.num_dim || dims[1]<0)
                {
                    snprintf(ei->msg,31, "3D JA dim. out of bounds");
                    ei->success=0;
                    return (link){};
                }
                if(dims[2]>=req.dims[dims[1]] || dims[2]<0)
                {
                    snprintf(ei->msg,31, "3D JA dim. out of bounds");
                    ei->success=0;
                    return (link){};
                }
            }

            l1.arr_info = PRIMITIVE;
            l1.type.prim_info = l1.type.rect_arr_info.betype;   // after indexing array element, it is a primitive integer
        }
    }
    return l1;
}


parseTree* cornerLexeme(parseTree* root,int idx)
{
    if(idx==0)
    {
        while(root->num_children)
        {
            root = root->children[root->num_children-1];
        }
        return root;
    }
    if(idx==1)
    {
        while(root->num_children)
        {
            root = root->children[0];
        }
        return root;
    }
    return root;
}


link get_data_type_right(parseTree * tree, typeExpressionTable * table, ErrInfo* ei) // gets data type to right of assignment statement
{
    ei->depth += 1;
    if(!ei->success) return (ei->depth-=1, (link){}); 

    int orig_depth = ei->depth;
    if(tree->term.type.nt==VAR) return (ei->depth-=1,get_data_type_var(tree, table, ei));
    ei->depth = orig_depth;

    if(tree->num_children==0) // needed for <INT> tokens
    {
        char* lexeme = tree->term.type.tok.lexeme;
        Token tk = tree->term.type.tok.token;
        if(tk==INT)
        {
            type_exp te = {INTEGER};
            link l;
            l.arr_info = PRIMITIVE;
            l.arr_storage = STATIC;
            l.type = te;
            tree->type_info = l;
            return (ei->depth-=1, l);
        }

        link* l = get_link(table,lexeme);
        if(!l)
        {
            snprintf(ei->msg, 31, "undeclared variable");
            strcpy(ei->lex1 , lexeme);
            get_str(tree->term.type,ei->type1,tree->term.is_term);
            ei->success=0;
            return (ei->depth-=1, (link){});   
        }
        tree->type_info = *l;
        return (ei->depth-=1, *l);
    }

    link d_left;
    link d_right;

    d_left = get_data_type_right(tree->children[0],table,ei);
    if(!ei->success) return (ei->depth-=1, (link){}); 

    if(tree->num_children==3)
    {

        get_str(tree->children[1]->term.type,ei->operator,tree->children[1]->term.is_term);
        parseTree* leftlexeme = cornerLexeme(tree->children[0],0);
        parseTree* rightlexeme = cornerLexeme(tree->children[tree->num_children-1],1);

        //printf("lexemes %s and %s \n", leftlexeme->term.type.tok.lexeme,rightlexeme->term.type.tok.lexeme);

        d_right = get_data_type_right(tree->children[2],table,ei);

        strcpy(ei->lex1,leftlexeme->term.type.tok.lexeme);
        get_str(leftlexeme->term.type,ei->type1,leftlexeme->term.is_term);

        strcpy(ei->lex2,rightlexeme->term.type.tok.lexeme);
        get_str(rightlexeme->term.type,ei->type2,rightlexeme->term.is_term);
        if(!ei->success) return (ei->depth-=1, (link){});  

        ei->success = is_op_compatible(d_left,d_right,tree->children[1]->term.type.tok.token,ei);
        strcpy(ei->lex1,leftlexeme->term.type.tok.lexeme);
        get_str(leftlexeme->term.type,ei->type1,leftlexeme->term.is_term);

        strcpy(ei->lex2,rightlexeme->term.type.tok.lexeme);
        get_str(rightlexeme->term.type,ei->type2,rightlexeme->term.is_term);
        if(!ei->success) return (ei->depth-=1, (link){});  
        
        if(tree->children[1]->term.is_term==1 && tree->children[1]->term.type.tok.token==OP_SLASH)
        {
            d_left.type.prim_info=REAL;
        }

    }

    tree->type_info = d_left;
    return (ei->depth-=1, tree->type_info);
}

void traverseAssigns(parseTree * tree, typeExpressionTable * table,int depth) {
    if(!(tree->term.is_term == 0 && tree->term.type.nt == ASSGN_STMT)) {
        for(int i = 0; i < tree->num_children; i++) traverseAssigns(tree->children[i], table,depth+1);
        return;
    }

    ErrInfo* ei = malloc(sizeof(ErrInfo));
    ei->success=1;
    strcpy(ei->stmt_type, "assignment");
    strcpy(ei->operator,"***");
    strcpy(ei->lex1,"na");
    strcpy(ei->type1,"na");
    strcpy(ei->lex2,"na");
    strcpy(ei->type2,"na");
    ei->depth=depth;

    link type_left = get_data_type_var(tree->children[0], table, ei);

    if(!ei->success)
    {
        printf("line : %d | stmt : %-11s | operator : %8s | lexeme1 : %s | type1 : %s | lexeme2 : %s | type2 : %s | depth : %d | message : %s\n", 
                ei->line_number,ei->stmt_type,ei->operator,ei->lex1,ei->type1,ei->lex2,ei->type2,ei->depth,ei->msg);
        return;
    }

    link type_right = get_data_type_right(tree->children[2],table,ei); 

    if(!ei->success)
    {
        printf("line : %d | stmt : %-11s | operator : %8s | lexeme1 : %s | type1 : %s | lexeme2 : %s | type2 : %s | depth : %d | message : %s\n", 
                ei->line_number,ei->stmt_type,ei->operator,ei->lex1,ei->type1,ei->lex2,ei->type2,ei->depth,ei->msg);
        return;
    }

    ei->success = is_op_compatible(type_left,type_right,ASSGN,ei);

    strcpy(ei->operator,"=");
    parseTree* leftlexeme = cornerLexeme(tree->children[0],0);
    parseTree* rightlexeme = cornerLexeme(tree->children[2],1);
    strcpy(ei->lex1,leftlexeme->term.type.tok.lexeme);
    get_str(leftlexeme->term.type,ei->type1,leftlexeme->term.is_term);
    strcpy(ei->lex2,rightlexeme->term.type.tok.lexeme);
    get_str(rightlexeme->term.type,ei->type2,rightlexeme->term.is_term);

    if(!ei->success)
    {
        printf("line : %d | stmt : %-11s | operator : %8s | lexeme1 : %s | type1 : %s | lexeme2 : %s | type2 : %s | depth : %d | message : %s\n", 
                ei->line_number,ei->stmt_type,ei->operator,ei->lex1,ei->type1,ei->lex2,ei->type2,ei->depth,ei->msg);
        return;
    }
}


void traverseParseTree(parseTree *t, typeExpressionTable *Table) {
    printf("Starting tree traversal\n\nErrors:\n");
    traverseDeclares(t, Table);
    traverseAssigns(t, Table,0);
    printf("Finished tree traversal\n");
}
