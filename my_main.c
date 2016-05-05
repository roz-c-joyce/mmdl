/*========== my_main.c ==========

  This is the only file you need to modify in order
  to get a working mdl project (for now).

  my_main.c will serve as the interpreter for mdl.
  When an mdl script goes through a lexer and parser, 
  the resulting operations will be in the array op[].

  Your job is to go through each entry in op and perform
  the required action from the list below:

  push: push a new origin matrix onto the origin stack
  pop: remove the top matrix on the origin stack

  move/scale/rotate: create a transformation matrix 
                     based on the provided values, then 
		     multiply the current top of the
		     origins stack by it.

  box/sphere/torus: create a solid object based on the
                    provided values. Store that in a 
		    temporary matrix, multiply it by the
		    current top of the origins stack, then
		    call draw_polygons.

  line: create a line based on the provided values. Store 
        that in a temporary matrix, multiply it by the
	current top of the origins stack, then call draw_lines.

  save: call save_extension with the provided filename

  display: view the image live
  
  jdyrlandweaver
  =========================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "parser.h"
#include "symtab.h"
#include "y.tab.h"

#include "misc_headers.h"
#include "matrix.h"
#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "stack.h"

void my_main( int polygons ) {

  int i;
  double step;
  double xval, yval, zval, xcor, ycor, zcor, andlge, radius;
  struct matrix *transform;
  struct matrix *tmp;
  screen t;
  color g;
  g.red = 0;
  g.green = 255;
  g.blue = 0;
  struct stack *s;
  
  s = new_stack();
  tmp = new_matrix(4, 1000);
  clear_screen( t );

  for (i=0;i<lastop;i++) {  
    printf("loop %d\n", i);
    switch (op[i].opcode){
    case PUSH:
      push(s);
      printf("%s\n", "pop\n");
      break;
    case POP:
      pop(origins);
      printf("%s\n", "pop\n");
      break;
    case MOVE:
      xval = op[i].op.move.d[0];
      yval = op[i].op.move.d[1];
      zval = op[i].op.move.d[2];
      tmp = make_translate(xval, yval, zval);
      matrix_mult(s->data[s->top], tmp);
      copy_matrix(tmp, s->data[s->top]);
      printf("%s\n", "move\n");
      break;
    case SCALE:
      xval = op[i].op.scale.d[0];
      yval = op[i].op.scale.d[1];
      zval = op[i].op.scale.d[2];
      tmp = make_sca;e(xval, yval, zval);
      matrix_mult(s->data[s->top], tmp);
      copy_matrix(tmp, s->data[s->top]);
      printf("%s\n", "scale\n");
      break;
    case ROTATE:
      angle = op[i].op.rotate.degrees * M_PI / 180;
      if(op[i].op.rotate.axis == 0){
	tmp = make_rotX(angle);
	matrix_mult(s->data[s->top], tmp);
	copy_matrix(tmp, s->data[s->s]);
      }
      else if(op[i].op.rotate.axis == 1){
	tmp = make_rotY(angle);
	matrix_mult(s->data[s->top], tmp);
	copy_matrix(tmp, s->data[s->top]);
      }
      else{
	tmp = make_rotZ(angle);
	matrix_mult(s->[origins->top], tmp);
	copy_matrix(tmp, s->data[s->top]);
      }
      printf("%s\n", "rotate\n");
      break;
    case BOX:
      xval = op[i].op.box.d0[0];
      yval = op[i].op.box.d0[1];
      zval = op[i].op.box.d0[2];
      xcor = op[i].op.box.d0[0];
      ycor = op[i].op.box.d0[1];
      zcor = op[i].op.box.d0[2];
      add_box(tmp, xval, yval, zval, vcor, ycor, zcor);
      matrix_mult(s->data[s->top], tmp);
      draw_polygons(tmp, t, g);
      printf("box\n");
      break;

    case SPHERE:
      xval = op[i].op.sphere.d[0];
      yval = op[i].op.sphere.d[1];
      zval = op[i].op.sphere.d[2];
      radius = op[i].op.sphere.r;
      add_sphere(tmp, xval, yval, zval, radius, 10);
      matrix_mult(s->data[s->top], tmp);
      draw_polygons(tmp, t, g);
      printf("%s\n", "sphere\n");
      break;
    case TORUS:
      xval = op[i].op.torus.d[0];
      yval = op[i].op.torus.d[1];
      zval = op[i].op.torus.d[2];
      double inner = op[i].op.torus.r0;
      double outer = op[i].op.torus.r1;
      add_torus(tmp, xval, yval, zval, inner, outer, 10);
      matrix_mult(s->data[s->top], tmp);
      draw_polygons(tmp, t, g);
      printf("%s\n", "torus\n");
      break;
    case LINE:
      xval = op[i].op.line.d[0];
      yval = op[i].op.line.d[1];
      zval = op[i].op.line.d[2];
      xcor = op[i].op.line.d[0];
      ycor = op[i].op.line.d[1];
      zcor = op[i].op.line.d[2];
      add_edge(tmp, xval, yval, zval, xcor, ycor, zcor);
      matrix_mult(s->data[s->top], tmp);
      draw_lines(tmp, t, g);
      printf("%s\n", "line\n");
      break;
    case SAVE:
      save_extension(t, op[i].op.save.p->name);
      printf("%s\n", "save\n");
      break;
    case DISPLAY:
      display(t);
      printf("%s\n", "display\n");
    }
  }
}
