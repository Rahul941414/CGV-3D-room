/**
 * @file bedroom.cpp
 * @brief Implementation of a bedroom scene using OpenGL.
 *
 * This file contains the implementation of a bedroom scene using OpenGL.
 * It includes functions to draw various objects such as cubes, trapezoids, and pyramids.
 * The scene includes lighting effects and texture mapping.
 */

/* reference for this project is taken from https://github.com/n-gauhar/3D-bedroom
*/


// This line silences deprecation warnings for OpenGL, as some functions may be deprecated in newer versions
#define GL_SILENCE_DEPRECATION

// Include the appropriate header file for GLUT based on the platform
#ifdef __APPLE_CC__
#include <GLUT/glut.h> // For macOS
#else
#include <GL/glut.h> // For other platforms
#endif

#include <stdlib.h> // Standard C library
#include "SOIL.h" // SOIL image loading library
#include <stdio.h> // Standard C I/O library
#include <iostream> // Standard C++ I/O library

// Global variables for flagging various states and window dimensions
GLboolean redFlag = true, switchOne = false, switchTwo = false, switchLamp = false,
    amb1 = true, diff1 = true, spec1 = true, amb2 = true, diff2 = true, spec2 = true,
    amb3 = true, diff3 = true, spec3 = true;
double windowHeight = 800, windowWidth = 600;
double eyeX = 7.0, eyeY = 2.0, eyeZ = 15.0, refX = 0, refY = 0, refZ = 0;
double theta = 180.0, y = 1.36, z = 7.97888;

// Function to load an image file as an OpenGL texture
GLuint loadTexture(const char* filename)
{
    // Use SOIL to load the image into an OpenGL texture
    GLuint texture = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    
    // Check if loading was successful
    if (texture == 0)
        std::cerr << "SOIL loading error: " << SOIL_last_result() << std::endl;
    // std::cout << texture << std::endl; // Output the texture ID for debugging
    return texture; // Return the texture ID
}


static GLfloat v_cube[8][3] =
{
    {0.0, 0.0, 0.0}, // Vertex 0
    {0.0, 0.0, 3.0}, // Vertex 1
    {3.0, 0.0, 3.0}, // Vertex 2
    {3.0, 0.0, 0.0}, // Vertex 3
    {0.0, 3.0, 0.0}, // Vertex 4
    {0.0, 3.0, 3.0}, // Vertex 5
    {3.0, 3.0, 3.0}, // Vertex 6
    {3.0, 3.0, 0.0}  // Vertex 7
};

static GLubyte quadIndices[6][4] =
{
    {0, 1, 2, 3}, // Bottom face indices
    {4, 5, 6, 7}, // Top face indices
    {5, 1, 2, 6}, // Front face indices
    {0, 4, 7, 3}, // Back face indices (specified in clockwise order)
    {2, 3, 7, 6}, // Right face indices
    {1, 5, 4, 0}  // Left face indices (specified in clockwise order)
};

// Function to calculate and set the normal vector for a triangle given three points
static void getNormal3p
(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3)
{
    // Calculate vectors U and V
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;
    Ux = x2 - x1;
    Uy = y2 - y1;
    Uz = z2 - z1;
    Vx = x3 - x1;
    Vy = y3 - y1;
    Vz = z3 - z1;

    // Calculate the cross product of U and V to get the normal vector
    Nx = Uy * Vz - Uz * Vy;
    Ny = Uz * Vx - Ux * Vz;
    Nz = Ux * Vy - Uy * Vx;

    // Set the normal vector
    glNormal3f(Nx, Ny, Nz);
}

// Function to draw a cube using GL_QUADS
void drawCube()
{
    glBegin(GL_QUADS);
    for (GLint i = 0; i < 6; i++)
    {
        // Calculate the normal vector for the current quad face
        getNormal3p(v_cube[quadIndices[i][0]][0], v_cube[quadIndices[i][0]][1], v_cube[quadIndices[i][0]][2],
                    v_cube[quadIndices[i][1]][0], v_cube[quadIndices[i][1]][1], v_cube[quadIndices[i][1]][2],
                    v_cube[quadIndices[i][2]][0], v_cube[quadIndices[i][2]][1], v_cube[quadIndices[i][2]][2]);
        
        // Specify vertices of the quad face
        glVertex3fv(&v_cube[quadIndices[i][0]][0]);
        glVertex3fv(&v_cube[quadIndices[i][1]][0]);
        glVertex3fv(&v_cube[quadIndices[i][2]][0]);
        glVertex3fv(&v_cube[quadIndices[i][3]][0]);
    }
    glEnd();
}


// Function to draw a cube with customizable material properties
void drawCube1(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX = 0, GLfloat ambY = 0, GLfloat ambZ = 0, GLfloat shine = 50)
{
    // Define material properties for the cube
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { shine };

    // Set the material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    // Draw the cube using GL_QUADS
    glBegin(GL_QUADS);
    for (GLint i = 0; i < 6; i++)
    {
        // Calculate and set the normal vector for the current quad face
        getNormal3p(v_cube[quadIndices[i][0]][0], v_cube[quadIndices[i][0]][1], v_cube[quadIndices[i][0]][2],
            v_cube[quadIndices[i][1]][0], v_cube[quadIndices[i][1]][1], v_cube[quadIndices[i][1]][2],
            v_cube[quadIndices[i][2]][0], v_cube[quadIndices[i][2]][1], v_cube[quadIndices[i][2]][2]);

        // Specify vertices of the quad face
        glVertex3fv(&v_cube[quadIndices[i][0]][0]);
        glVertex3fv(&v_cube[quadIndices[i][1]][0]);
        glVertex3fv(&v_cube[quadIndices[i][2]][0]);
        glVertex3fv(&v_cube[quadIndices[i][3]][0]);
    }
    glEnd();
}

// Function to draw a carpet with customizable material properties and texture
void drawCarpet(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX = 0, GLfloat ambY = 0, GLfloat ambZ = 0, GLfloat shine = 50)
{
    // Push the current matrix onto the stack and save current state
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    // Load and enable the texture for the carpet
    GLuint texture = loadTexture("image.png");
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Define material properties for the carpet
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { shine };

    // Set the material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    // Draw the carpet using GL_QUADS
    glBegin(GL_QUADS);
    for (GLint i = 0; i < 6; i++)
    {
        // Calculate and set the normal vector for the current quad face
        getNormal3p(v_cube[quadIndices[i][0]][0], v_cube[quadIndices[i][0]][1], v_cube[quadIndices[i][0]][2],
            v_cube[quadIndices[i][1]][0], v_cube[quadIndices[i][1]][1], v_cube[quadIndices[i][1]][2],
            v_cube[quadIndices[i][2]][0], v_cube[quadIndices[i][2]][1], v_cube[quadIndices[i][2]][2]);

        // Specify vertices of the quad face with texture coordinates
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(&v_cube[quadIndices[i][0]][0]);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(&v_cube[quadIndices[i][1]][0]);
        glTexCoord2f(1.0f, 1.0f); glVertex3fv(&v_cube[quadIndices[i][2]][0]);
        glTexCoord2f(0.0f, 1.0f); glVertex3fv(&v_cube[quadIndices[i][3]][0]);
    }
    glEnd();

    // Disable texture and restore previous state
    glDisable(GL_TEXTURE_2D);
    glPopAttrib();
    glPopMatrix();
}

// Definition of vertices for a trapezoid
static GLfloat v_trapezoid[8][3] =
{
    {0.0, 0.0, 0.0}, //0
    {0.0, 0.0, 3.0}, //1
    {3.0, 0.0, 3.0}, //2
    {3.0, 0.0, 0.0}, //3
    {0.5, 3.0, 0.5}, //4
    {0.5, 3.0, 2.5}, //5
    {2.5, 3.0, 2.5}, //6
    {2.5, 3.0, 0.5}  //7
};

// Definition of quad indices for a trapezoid
static GLubyte TquadIndices[6][4] =
{
    {0, 1, 2, 3}, // Bottom face indices
    {4, 5, 6, 7}, // Top face indices
    {5, 1, 2, 6}, // Front face indices
    {0, 4, 7, 3}, // Back face indices (specified in clockwise order)
    {2, 3, 7, 6}, // Right face indices
    {1, 5, 4, 0}  // Left face indices (specified in clockwise order)
};

// Function to draw a trapezoid with customizable material properties
void drawTrapezoid(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine = 50)
{
    // Define material properties for the trapezoid
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_emission[] = { difX, difY, difZ, 0.0 };
    GLfloat mat_shininess[] = { shine };

    // Set the material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // If the switchLamp flag is true, set emission material property
    if (switchLamp == true) {
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    }
    else {
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }

    // Draw the trapezoid using GL_QUADS
    glBegin(GL_QUADS);
    for (GLint i = 0; i < 6; i++)
    {
        // Calculate and set the normal vector for the current quad face
        getNormal3p(v_trapezoid[TquadIndices[i][0]][0], v_trapezoid[TquadIndices[i][0]][1], v_trapezoid[TquadIndices[i][0]][2],
            v_trapezoid[TquadIndices[i][1]][0], v_trapezoid[TquadIndices[i][1]][1], v_trapezoid[TquadIndices[i][1]][2],
            v_trapezoid[TquadIndices[i][2]][0], v_trapezoid[TquadIndices[i][2]][1], v_trapezoid[TquadIndices[i][2]][2]);

        // Specify vertices of the quad face
        glVertex3fv(&v_trapezoid[TquadIndices[i][0]][0]);
        glVertex3fv(&v_trapezoid[TquadIndices[i][1]][0]);
        glVertex3fv(&v_trapezoid[TquadIndices[i][2]][0]);
        glVertex3fv(&v_trapezoid[TquadIndices[i][3]][0]);
    }
    glEnd();
}



// Definition of vertices for a pyramid
static GLfloat v_pyramid[5][3] =
{
    {0.0, 0.0, 0.0}, // Vertex 0
    {0.0, 0.0, 2.0}, // Vertex 1
    {2.0, 0.0, 2.0}, // Vertex 2
    {2.0, 0.0, 0.0}, // Vertex 3
    {1.0, 4.0, 1.0}  // Vertex 4 (apex)
};

// Definition of triangle indices for the pyramid's faces
static GLubyte p_Indices[4][3] =
{
    {4, 1, 2}, // Front face indices
    {4, 2, 3}, // Right face indices
    {4, 3, 0}, // Back face indices
    {4, 0, 1}  // Left face indices
};

// Definition of quad indices for the base of the pyramid
static GLubyte PquadIndices[1][4] =
{
    {0, 3, 2, 1} // Base indices
};

// Function to draw a pyramid with customizable material properties
void drawpyramid(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine)
{
    // Define material properties for the pyramid
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { shine };

    // Set the material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Draw the triangular faces of the pyramid
    glBegin(GL_TRIANGLES);
    for (GLint i = 0; i < 4; i++)
    {
        // Calculate and set the normal vector for the current triangle face
        getNormal3p(v_pyramid[p_Indices[i][0]][0], v_pyramid[p_Indices[i][0]][1], v_pyramid[p_Indices[i][0]][2],
            v_pyramid[p_Indices[i][1]][0], v_pyramid[p_Indices[i][1]][1], v_pyramid[p_Indices[i][1]][2],
            v_pyramid[p_Indices[i][2]][0], v_pyramid[p_Indices[i][2]][1], v_pyramid[p_Indices[i][2]][2]);

        // Specify vertices of the current triangle face
        glVertex3fv(&v_pyramid[p_Indices[i][0]][0]);
        glVertex3fv(&v_pyramid[p_Indices[i][1]][0]);
        glVertex3fv(&v_pyramid[p_Indices[i][2]][0]);
    }
    glEnd();

    // Draw the base of the pyramid
    glBegin(GL_QUADS);
    for (GLint i = 0; i < 1; i++)
    {
        // Calculate and set the normal vector for the base quad
        getNormal3p(v_pyramid[PquadIndices[i][0]][0], v_pyramid[PquadIndices[i][0]][1], v_pyramid[PquadIndices[i][0]][2],
            v_pyramid[PquadIndices[i][1]][0], v_pyramid[PquadIndices[i][1]][1], v_pyramid[PquadIndices[i][1]][2],
            v_pyramid[PquadIndices[i][2]][0], v_pyramid[PquadIndices[i][2]][1], v_pyramid[PquadIndices[i][2]][2]);

        // Specify vertices of the base quad
        glVertex3fv(&v_pyramid[PquadIndices[i][0]][0]);
        glVertex3fv(&v_pyramid[PquadIndices[i][1]][0]);
        glVertex3fv(&v_pyramid[PquadIndices[i][2]][0]);
        glVertex3fv(&v_pyramid[PquadIndices[i][3]][0]);
    }
    glEnd();
}

// Function to draw a custom polygon with customizable material properties
void polygon(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine)
{
    // Define material properties for the polygon
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { shine };

    // Set the material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Draw the polygon using GL_POLYGON
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    glVertex2f(6, 0);
    glVertex2f(5.8, 1);
    glVertex2f(5.2, 2);
    glVertex2f(5, 2.2);
    glVertex2f(4, 2.8);
    glVertex2f(3, 3);
    glVertex2f(2, 2.8);
    glVertex2f(1, 2.2);
    glVertex2f(0.8, 2);
    glVertex2f(0.2, 1);
    glEnd();
}

// Function to draw a polygon with lines
void polygonLine(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine)
{
    // Define material properties for the polygon
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { shine };

    // Set the material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Begin drawing lines for the polygon
    glBegin(GL_LINE_STRIP);
    glVertex2f(6, 0);
    glVertex2f(5.8, 1);
    glVertex2f(5.2, 2);
    glVertex2f(5, 2.2);
    glVertex2f(4, 2.8);
    glVertex2f(3, 3);
    glVertex2f(2, 2.8);
    glVertex2f(1, 2.2);
    glVertex2f(0.8, 2);
    glVertex2f(0.2, 1);
    glVertex2f(0, 0);
    glEnd();
}

// Function to draw a sphere with customizable material properties
void drawSphere(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine)
{
    // Define material properties for the sphere
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { shine };

    // Set the material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Draw a solid sphere
    glutSolidSphere(3.0, 20, 16);
}

// Function to draw a cupboard
void cupboard()
{
    // Cupboard/Almari ************************************************************

    // Cupboard
    glPushMatrix();
    glTranslatef(4, 0, 4.4);
    glScalef(0.5, 1, 0.5);
    drawCube1(0.5, 0.2, 0.2, 0.25, 0.1, 0.1);
    glPopMatrix();

    // Cupboard's vertical striplines
    glPushMatrix();
    glTranslatef(4, 1, 5.9);
    glScalef(0.5, 0.01, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4, 0.5, 5.9);
    glScalef(0.5, 0.01, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4, 0, 5.9);
    glScalef(0.5, 0.01, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    // Cupboard's horizontal striplines
    glPushMatrix();
    glTranslatef(5.5, 0, 5.9);
    glScalef(0.01, 1, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4.75, 1, 5.9);
    glScalef(0.01, 0.67, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4, 0, 5.9);
    glScalef(0.01, 1, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    // Cupboard's handles
    glPushMatrix();
    glTranslatef(5, 1.4, 5.9);
    glScalef(0.02, 0.18, 0.01);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    // Sphere for the cupboard's handle
    glPushMatrix();
    glTranslatef(5.02, 1.9, 5.91);
    glScalef(0.02, 0.02, 0.01);
    drawSphere(0.2, 0.1, 0.1, 0.1, 0.05, 0.05, 10);
    glPopMatrix();

    // Left handle
    glPushMatrix();
    glTranslatef(4.5, 1.4, 5.9);
    glScalef(0.02, 0.18, 0.01);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    // Sphere for the left handle
    glPushMatrix();
    glTranslatef(4.52, 1.9, 5.91);
    glScalef(0.02, 0.02, 0.01);
    drawSphere(0.2, 0.1, 0.1, 0.1, 0.05, 0.05, 10);
    glPopMatrix();

    // Drawer handles
    glPushMatrix();
    glTranslatef(4.5, 0.7, 5.9);
    glScalef(0.16, 0.02, 0.01);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4.5, 0.25, 5.9);
    glScalef(0.16, 0.02, 0.01);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();
}


// Function to draw the room
void room()
{
    // Carpet
    glPushMatrix();
    glTranslatef(3, -0.2, 7);
    glScalef(1.3, 0.01, 1.7);
    drawCarpet(0.4, 0.1, 0.0, 0.20, 0.05, 0.0);
    glPopMatrix();
    
    // Right wall
    glPushMatrix();
    glTranslatef(-1.5, -1, 0.5);
    glScalef(5, 2, 0.1);
    drawCube1(1, 0.8, 0.7, 0.5, 0.4, 0.35);
    glPopMatrix();
    
    // Left wall
    glPushMatrix();
    glTranslatef(-4.5, -1, 0);
    glScalef(1, 2, 5);
    drawCube1(1, 0.8, 0.7, 0.5, 0.4, 0.35);
    glPopMatrix();
    
    // Wall besides the right wall
    glPushMatrix();
    glTranslatef(8, -1, 0);
    glScalef(0.2, 2, 5);
    drawCube1(1, 0.8, 0.7, 0.5, 0.4, 0.35);
    glPopMatrix();
    
    // Ceiling
    glPushMatrix();
    glTranslatef(-2, 5.1, 0);
    glScalef(5, 0.1, 7);
    drawCube1(1.0, 0.9, 0.8, 0.5, 0.45, 0.4);
    glPopMatrix();
    
    // Floor
    glPushMatrix();
    glTranslatef(-1, -5, 0);
    glScalef(5, 0.1, 7);
    drawCube1(0.5, 0.1, 0.0, 0.25, 0.05, 0);
    glPopMatrix();
}

// Function to draw the bed
void bed()
{
    // Bed headboard
    glPushMatrix();
    glTranslatef(-2, -0.5, 6.2);
    glScalef(0.1, 0.5, 0.9);
    drawCube1(0.5, 0.2, 0.2, 0.25, 0.1, 0.1);
    glPopMatrix();
    
    // Bed body
    glPushMatrix();
    glTranslatef(0, -0.5, 6.2);
    glScalef(1, 0.2, 0.9);
    drawCube1(0.824, 0.706, 0.549, 0.412, 0.353, 0.2745);
    glPopMatrix();
    
    // Pillows
    glPushMatrix();
    glTranslatef(0.5, 0.5, 6);
    glRotatef(20, 0, 0, 1);
    glScalef(0.1, 0.15, 0.28);
    drawCube1(0.627, 0.322, 0.176, 0.3135, 0.161, 0.088);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.5, 0.5, 7.2);
    glRotatef(22, 0, 0, 1);
    glScalef(0.1, 0.15, 0.28);
    drawCube1(0.627, 0.322, 0.176, 0.3135, 0.161, 0.088);
    glPopMatrix();
    
    // Blanket
    glPushMatrix();
    glTranslatef(1.4, 0.45, 5.5);
    glScalef(0.5, 0.05, 0.95);
    drawCube1(0.627, 0.322, 0.176, 0.3135, 0.161, 0.088);
    glPopMatrix();
    
    // Blanket side left part
    glPushMatrix();
    glTranslatef(1.4, -0.3, 8.16);
    glScalef(0.5, 0.25, 0.05);
    drawCube1(0.627, 0.322, 0.176, 0.3135, 0.161, 0.088);
    glPopMatrix();
}

// Function to draw the bedside drawer
void bedsideDrawer()
{
    // Bedside drawer
    glPushMatrix();
    glTranslatef(0.5, -0.1, 8.7);
    glScalef(0.12, 0.2, 0.23);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();
    
    // Side drawer's drawer
    glPushMatrix();
    glTranslatef(0.88, 0, 8.8);
    glScalef(0.0001, 0.11, 0.18);
    drawCube1(0.3, 0.2, 0.2, 0.15, 0.1, 0.1);
    glPopMatrix();
    
    // Side drawer's knob
    glPushMatrix();
    glTranslatef(0.9, 0.15, 9.05);
    glScalef(0.01, 0.02, 0.02);
    drawSphere(0.3, 0.1, 0.0, 0.15, 0.05, 0.0);
    glPopMatrix();
}


// Function to draw the lamp
void lamp()
{
    // Lamp base
    glPushMatrix();
    glTranslatef(0.6, 0.5, 8.95);
    glScalef(0.07, 0.02, 0.07);
    drawCube1(0, 0, 1, 0, 0, 0.5);
    glPopMatrix();
    
    // Lamp stand
    glColor3f(1, 0, 0);
    glPushMatrix();
    glTranslatef(0.7, 0.35, 9.05);
    glScalef(0.01, 0.2, 0.01);
    drawCube1(1, 0, 0, 0.5, 0.0, 0.0);
    glPopMatrix();
        
    // Lamp shade
    glColor3f(0.000, 0.000, 0.545);
    glPushMatrix();
    glTranslatef(0.6, 0.9, 8.9);
    glScalef(0.08, 0.09, 0.08);
    drawTrapezoid(0.000, 0.000, 0.545, 0, 0, 0.2725);
    glPopMatrix();
}

// Function to draw the Linkin Park poster
void LinkinParkPoster()
{
    // Poster black background
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(-1, 1.4, 4.6);
    glScalef(0.0001, 0.65, 0.8);
    drawCube1(0, 0, 0, 0, 0, 0, 10);
    glPopMatrix();
    
    // Linkin Park logo components
    glColor3f(1.0, 1.0, 1.0);
    
    // First component
    glPushMatrix();
    glTranslatef(-0.9, 2.1, 5.5);
    glScalef(0.0001, 0.02, 0.25);
    drawCube1(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 10);
    glPopMatrix();
    
    // Second component
    glPushMatrix();
    glTranslatef(-0.9, 2.1, 6.2);
    glRotatef(-14, 1, 0, 0);
    glScalef(0.0001, 0.28, 0.02);
    drawCube1(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 10);
    glPopMatrix();
    
    // Third component
    glPushMatrix();
    glTranslatef(-0.9, 1.8, 6);
    glRotatef(-14, 1, 0, 0);
    glScalef(0.0001, 0.29, 0.02);
    drawCube1(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 10);
    glPopMatrix();
    
    // Fourth component
    glPushMatrix();
    glTranslatef(-0.9, 2.1, 5.5);
    glRotatef(23, 1, 0, 0);
    glScalef(0.0001, 0.25, 0.02);
    drawCube1(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 10);
    glPopMatrix();
}

// Function to draw the wardrobe
void wardrobe()
{
    // Wardrobe
    glPushMatrix();
    glTranslatef(0, 0, 4);
    glScalef(0.12, 0.6, 0.4);
    drawCube1(0.3, 0.1, 0, 0.15, 0.05, 0);
    glPopMatrix();
    
    // Wardrobe's drawers
    for (float yPos = 1.4; yPos >= 0.2; yPos -= 0.4)
    {
        glPushMatrix();
        glTranslatef(0.36, yPos, 4.05);
        glScalef(0.0001, 0.11, 0.38);
        drawCube1(0.5, 0.2, 0.2, 0.25, 0.1, 0.1);
        glPopMatrix();
    }
    
    // Wardrobe's drawer handles
    glColor3f(0.3, 0.1, 0);
    for (float yPos = 1.5; yPos >= 0.3; yPos -= 0.4)
    {
        glPushMatrix();
        glTranslatef(0.37, yPos, 4.3);
        glScalef(0.01, 0.03, 0.2);
        drawCube1(0.3, 0.1, 0, 0.15, 0.05, 0.0);
        glPopMatrix();
    }
}

// Function to draw the dressing table
void dressingTable()
{
    // Dressing table main body ************************************************
    
    // Dressing table left body
    glPushMatrix();
    glTranslatef(5.9, 0, 4.6);
    glScalef(0.2, 0.2, 0.2);
    drawCube1(0.545, 0.271, 0.075, 0.2725, 0.1355, 0.0375);
    glPopMatrix();
    
    /* Commented out code for stripes on the left body
    // Dressing table left body left stripe
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(5.9, 0, 5.2);
    glScalef(0.01, 0.3, 0.0001);
    drawCube();
    glPopMatrix();
    
    // Dressing table left body right stripe
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(6.5, 0, 5.2);
    glScalef(0.01, 0.2, 0.0001);
    drawCube();
    glPopMatrix();
    
    // Dressing table left body bottom stripe
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(5.9, 0, 5.2);
    glScalef(0.2, 0.01, 0.0001);
    drawCube();
    glPopMatrix();
    */
    
    // Dressing table right body
    glPushMatrix();
    glTranslatef(7, 0, 4.6);
    glScalef(0.2, 0.2, 0.2);
    drawCube1(0.545, 0.271, 0.075, 0.2725, 0.1355, 0.0375);
    glPopMatrix();
    
    /* Commented out code for stripes on the right body
    // Dressing table right body left stripe
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(7, 0, 5.2);
    glScalef(0.01, 0.2, 0.0001);
    drawCube();
    glPopMatrix();
    
    // Dressing table right body right stripe
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(7.6, 0, 5.2);
    glScalef(0.01, 0.3, 0.0001);
    drawCube();
    glPopMatrix();
    
    // Dressing table right body bottom stripe
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(7, 0, 5.2);
    glScalef(0.2, 0.01, 0.0001);
    drawCube();
    glPopMatrix();
    */
    
    // Dressing table upper body
    glPushMatrix();
    glTranslatef(5.9, 0.6, 4.6);
    glScalef(0.57, 0.1, 0.2);
    drawCube1(0.545, 0.271, 0.075, 0.2725, 0.1355, 0.0375);
    glPopMatrix();
    
    // Dressing table upper body bottom stripe
    glPushMatrix();
    glTranslatef(5.9, 0.6, 5.2);
    glScalef(0.57, 0.01, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();
    
    // Dressing table upper body upper stripe
    glPushMatrix();
    glTranslatef(5.9, 0.9, 5.2);
    glScalef(0.57, 0.01, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();
    
    // Dressing table upper body handle
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(6.5, 0.75, 5.2);
    glScalef(0.16, 0.02, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();
    
    // Dressing table left body handle
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(6.4, 0.1, 5.2);
    glScalef(0.02, 0.13, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();
    
    // Dressing table right body handle
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(7.1, 0.1, 5.2);
    glScalef(0.02, 0.13, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();
    
    // Dressing table mirrors ************************************************
    
    // Dressing table main mirror
    glPushMatrix();
    glTranslatef(6.2, 0.9, 4.7);
    glScalef(0.36, 0.5, 0.0001);
    drawCube1(0.690, 0.878, 0.902, 0.345, 0.439, 0.451, 10);
    glPopMatrix();
    
    // Dressing table left mirror
    glPushMatrix();
    glTranslatef(5.92, 0.9, 4.7);
    glScalef(0.1, 0.48, 0.0001);
    drawCube1(0.690, 0.878, 0.902, 0.345, 0.439, 0.451, 10);
    glPopMatrix();
    
    // Commented out code for stripes on the left mirror
    // Dressing table left mirror left stripe
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(5.92, 0.9, 4.71);
    glScalef(0.019, 0.48, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();
    
    // Dressing table left mirror right stripe
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(6.17, 0.9, 4.71);
    glScalef(0.019, 0.48, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();
    
    // Dressing table mirror stripe
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(5.92, 0.9, 4.71);
    glScalef(0.55, 0.019, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();
    
    // Dressing table left mirror upper stripe
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(5.92, 2.3, 4.71);
    glScalef(0.1, 0.019, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();
    
    // Dressing table right mirror
    glPushMatrix();
    glTranslatef(7.25, 0.9, 4.7);
    glScalef(0.1, 0.48, 0.0001);
    drawCube1(0.690, 0.878, 0.902, 0.345, 0.439, 0.451, 10);
    glPopMatrix();
    
    // Dressing table left mirror upper stripe
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(7.25, 2.3, 4.71);
    glScalef(0.1, 0.019, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();
    
    // Dressing table right mirror left stripe
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(7.25, 0.9, 4.71);
    glScalef(0.019, 0.48, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();
    
    // Dressing table right mirror right stripe
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(7.5, 0.9, 4.71);
    glScalef(0.019, 0.48, 0.0001);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();
    
    // Dressing table main mirror polygon part
    glColor3f(0.690, 0.878, 0.902);
    glPushMatrix();
    glTranslatef(6.2, 2.4, 4.7);
    glScalef(0.18, 0.18, 2);
    polygon(0.690, 0.878, 0.902, 0.345, 0.439, 0.451, 10);
    glPopMatrix();
    
    // Dressing table upper round stripe
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(6.2, 2.4, 4.71);
    glScalef(0.18, 0.18, 1);
    polygonLine(0.2, 0.1, 0.1, 0.1, 0.05, 0.05, 50);
    glPopMatrix();
}


void wallshelf()
{
    // Wall Shelf ******************************************************

    // Wall shelf one
    glPushMatrix();
    glTranslatef(1.5, 2.7, 3);
    glScalef(0.4, 0.03, 0.2);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    // Wall shelf two
    glPushMatrix();
    glTranslatef(1, 2.3, 3);
    glScalef(0.4, 0.03, 0.2);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    // Wall shelf three
    glPushMatrix();
    glTranslatef(0.5, 1.9, 3);
    glScalef(0.4, 0.03, 0.2);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    // Wall shelf four
    glPushMatrix();
    glTranslatef(1, 1.5, 3);
    glScalef(0.4, 0.03, 0.2);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    // Wall shelf five
    glPushMatrix();
    glTranslatef(1.5, 1.1, 3);
    glScalef(0.4, 0.03, 0.2);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    // Showpiece on the bottom shelf from left 1
    glPushMatrix();
    glTranslatef(1.5, 1.2, 3);
    glScalef(0.04, 0.06, 0.2);
    drawCube1(0.698, 0.133, 0.133, 0.349, 0.0665, 0.0665);
    glPopMatrix();

    // Showpiece on the bottom shelf from left 2
    glPushMatrix();
    glTranslatef(2, 1.2, 3);
    glScalef(0.04, 0.06, 0.2);
    drawCube1(0.729, 0.333, 0.827, 0.3645, 0.1665, 0.4135);
    glPopMatrix();

    // Showpiece on the bottom shelf from left 3 lower portion
    glPushMatrix();
    glTranslatef(2.5, 1.2, 3);
    glScalef(0.04, 0.06, 0.2);
    drawCube1(0.098, 0.098, 0.439, 0.049, 0.049, 0.2195);
    glPopMatrix();

    // Showpiece on the bottom shelf from left 3 upper portion
    glPushMatrix();
    glTranslatef(2.51, 1.35, 3);
    glScalef(0.01, 0.05, 0.2);
    drawCube1(0.529, 0.808, 0.980, 0.2645, 0.404, 0.490);
    glPopMatrix();

    // Showpiece on the top shelf left 2
    glColor3f(0.502, 0.502, 0.000);
    glPushMatrix();
    glTranslatef(2.5, 2.71, 3);
    glScalef(0.05, 0.16, 0.01);
    drawCube1(0.502, 0.502, 0.000, 0.251, 0.251, 0);
    glPopMatrix();

    // Showpiece on the top shelf left 1
    glPushMatrix();
    glTranslatef(1.8, 2.71, 3);
    glScalef(0.16, 0.1, 0.01);
    drawCube1(0, 0, 0.9, 0, 0, 0.45);
    glPopMatrix();

    // Showpiece on 2nd shelf
    glColor3f(0.416, 0.353, 0.804);
    glPushMatrix();
    glTranslatef(1.3, 2.4, 3);
    glScalef(0.16, 0.08, 0.01);
    drawCube1(0.416, 0.353, 0.804, 0.208, 0.1765, 0.402);
    glPopMatrix();

    // Showpiece on 3rd shelf left 1
    glPushMatrix();
    glTranslatef(0.4, 1.9, 3);
    glScalef(0.05, 0.16, 0.01);
    drawCube1(0.863, 0.078, 0.235, 0.4315, 0.039, 0.1175);
    glPopMatrix();

    // Showpiece on 3rd shelf left 2
    glPushMatrix();
    glTranslatef(0.7, 1.9, 3);
    glScalef(0.05, 0.12, 0.01);
    drawCube1(0.780, 0.082, 0.522, 0.39, 0.041, 0.261);
    glPopMatrix();

    // Showpiece on 3rd shelf left 3
    glColor3f(0.600, 0.196, 0.800);
    glPushMatrix();
    glTranslatef(1, 1.9, 3);
    glScalef(0.05, 0.09, 0.01);
    drawCube1(0.6, 0.196, 0.8, 0.3, 0.098, 0.4);
    glPopMatrix();

    // Showpiece on 4th shelf
    glPushMatrix();
    glTranslatef(1.8, 1.5, 3);
    glScalef(0.2, 0.1, 0.2);
    drawpyramid(0.282, 0.239, 0.545, 0.141, 0.1195, 0.2725, 50);
    glPopMatrix();

    // Showpiece on 4th shelf
    glPushMatrix();
    glTranslatef(1.4, 1.5, 3);
    glScalef(0.15, 0.1, 0.2);
    drawpyramid(0.251, 0.878, 0.816, 0.1255, 0.439, 0.408, 50);
    glPopMatrix();
}

void Clock()
{
    // Clock ************************************************************

    // Clock body
    glColor3f(0.545, 0.271, 0.075);
    glPushMatrix();
    glTranslatef(-0.9, 1.8, 7.87);
    glScalef(0.08, 0.25, 0.1);
    drawCube1(0.545, 0.271, 0.075, 0.271, 0.1335, 0.0375, 50);
    glPopMatrix();

    // Clock body white
    glPushMatrix();
    glTranslatef(-0.83, 1.9, 7.9);
    glScalef(0.06, 0.2, 0.08);
    drawCube1(1.000, 0.894, 0.710, 1.000, 0.894, 0.710);
    glPopMatrix();

    // Clock hour handle
    glPushMatrix();
    glTranslatef(-0.65, 2.18, 8.01);
    glRotatef(45, 1, 0, 0);
    glScalef(0.0001, 0.01, 0.04);
    drawCube1(0, 0, 0, 0, 0, 0);
    glPopMatrix();

    // Clock minute handle
    glPushMatrix();
    glTranslatef(-0.65, 2.18, 8.01);
    glRotatef(90, 1, 0, 0);
    glScalef(0.0001, 0.012, 0.08);
    drawCube1(0, 0, 0, 0, 0, 0);
    glPopMatrix();

    // Clock pendulum stick
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(-0.7, 2, 8.1);
    glRotatef(theta, 1, 0, 0);
    glScalef(0.0001, 0.2, 0.03);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    // Clock pendulum ball
    glColor3f(0.2, 0.1, 0.1);
    glPushMatrix();
    glTranslatef(-0.72, 1.42, z);
    glScalef(0.035, 0.035, 0.035);
    drawSphere(0.2, 0.1, 0.1, 0.1, 0.05, 0.05, 10);
    glPopMatrix();

    // Clock top pyramid
    glPushMatrix();
    glTranslatef(-0.9, 2.5, 7.81);
    glScalef(0.16, 0.1, 0.2);
    drawpyramid(0.5, 0.2, 0, 0.25, 0.1, 0, 50);
    glPopMatrix();
}

void window()
{
    // Window *******************************************************

    // Window white open
    glPushMatrix();
    glTranslatef(-0.9, 1, 8.9);
    glScalef(0.0001, .6, .3);
    drawCube1(1.0, 1.0, 1.0, 0.05, 0.05, 0.05);
    glPopMatrix();

    // Window right side corner
    glPushMatrix();
    glTranslatef(-0.9, 1, 8.9);
    glScalef(0.04, 0.6, 0.0001);
    drawCube1(0.8, 0.6, 0.4, 0.4, 0.3, 0.2);
    glPopMatrix();

    // Window left side corner
    glPushMatrix();
    glTranslatef(-0.9, 1, 9.8);
    glScalef(0.04, 0.6, 0.0001);
    drawCube1(0.8, 0.6, 0.4, 0.4, 0.3, 0.2);
    glPopMatrix();

    // Window upper side corner
    glPushMatrix();
    glTranslatef(-0.7, 2.7, 8.9);
    glScalef(0.0001, 0.05, 0.4);
    drawCube1(0.7, 0.6, 0.5, 0.35, 0.3, 0.25);
    glPopMatrix();

    // Window lower side corner
    glPushMatrix();
    glTranslatef(-0.8, 1.02, 8.9);
    glScalef(0.0001, 0.02, 0.34);
    drawCube1(0.7, 0.6, 0.5, 0.35, 0.3, 0.25);
    glPopMatrix();

    // Window vertical bar 1
    glPushMatrix();
    glTranslatef(-0.87, 2.1, 8.9);
    glScalef(0.0001, 0.02, 0.3);
    drawCube1(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5);
    glPopMatrix();

    // Window vertical bar 2
    glPushMatrix();
    glTranslatef(-0.87, 1.6, 8.9);
    glScalef(0.0001, 0.02, 0.3);
    drawCube1(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5);
    glPopMatrix();

    // Window horizontal bar
    glPushMatrix();
    glTranslatef(-0.87, 1, 9.3);
    glScalef(0.0001, 0.6, 0.02);
    drawCube1(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5);
    glPopMatrix();
}


void sphericalObject()
{
    // Table top part
    glPushMatrix(); // Save current matrix
    glTranslatef(5, 0.2, 10); // Translate to table top position
    glScalef(0.1, 0.02, 0.1); // Scale the table top
    drawSphere(0.5, 0.2, 0, 0.25, 0.1, 0, 20); // Draw the table top sphere
    glPopMatrix(); // Restore previous matrix
    
    // Table leg
    glColor3f(0.2, 0.1, 0.1); // Set color for the table leg
    glPushMatrix(); // Save current matrix
    glTranslatef(4.98, -0.1, 10); // Translate to table leg position
    glScalef(0.02, 0.1, 0.02); // Scale the table leg
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05); // Draw the table leg cube
    glPopMatrix(); // Restore previous matrix
    
    // Base
    glPushMatrix(); // Save current matrix
    glTranslatef(5, -0.1, 10); // Translate to base position
    glScalef(0.05, 0.01, 0.05); // Scale the base
    drawSphere(0.5, 0.2, 0, 0.25, 0.1, 0, 20); // Draw the base sphere
    glPopMatrix(); // Restore previous matrix
}

void lightBulb1()
{
    // Material properties
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_diffuse[] = { 1.000, 0.843, 0.000, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat high_shininess[] = { 100.0 };
    GLfloat mat_emission[] = {1.000, 1, 1, 0.0};
    
    glPushMatrix(); // Save current matrix
    glTranslatef(5, 5, 8); // Translate to light bulb 1 position
    glScalef(0.2, 0.2, 0.2); // Scale the light bulb
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat); // Set ambient material property
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse); // Set diffuse material property
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat); // Set specular material property
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess); // Set shininess material property
    
    // Set emission material property based on switch state
    if (switchOne == true) {
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    } else {
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
    
    glutSolidSphere(1.0, 16, 16); // Draw the light bulb
    glPopMatrix(); // Restore previous matrix
}

// The comments explain each step of the function, such as translations, scaling, and drawing. This makes it easier for others to understand the code's purpose and behavior.


void lightBulb2()
{
    // Material properties
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 1.000, 0.843, 0.000, 1.0 };
    GLfloat high_shininess[] = { 100.0 };
    GLfloat mat_emission[] = {1, 1, 1, 1.0};
    
    glPushMatrix(); // Save current matrix
    glTranslatef(0, 5, 8); // Translate to light bulb 2 position
    glScalef(0.2, 0.2, 0.2); // Scale the light bulb
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat); // Set ambient material property
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse); // Set diffuse material property
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat); // Set specular material property
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess); // Set shininess material property
    
    // Set emission material property based on switch state
    if (switchTwo == true) {
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    } else {
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
    
    glutSolidSphere(1.0, 16, 16); // Draw the light bulb
    glPopMatrix(); // Restore previous matrix
}

void lightBulb3()
{
    // Material properties
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 1.000, 0.843, 0.000, 1.0 };
    GLfloat high_shininess[] = { 100.0 };
    GLfloat mat_emission[] = {1, 1, 1, 1.0};
    
    glPushMatrix(); // Save current matrix
    glTranslatef(0.7, 1.5, 9.0); // Translate to light bulb 3 position
    glScalef(0.2, 0.2, 0.2); // Scale the light bulb
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat); // Set ambient material property
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse); // Set diffuse material property
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat); // Set specular material property
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess); // Set shininess material property
    
    // Set emission material property based on switch state
    if (switchLamp == true) {
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    } else {
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
    
    glutSolidSphere(1.0, 16, 16); // Draw the light bulb
    glPopMatrix(); // Restore previous matrix
}

void lightOne()
{
    glPushMatrix(); // Save current matrix
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {5.0, 5.0, 8.0, 1.0}; // Set light 1 position
    
    // glEnable(GL_LIGHT0);
    
    // Set light properties based on switch state
    if (amb1 == true) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    } else {
        glLightfv(GL_LIGHT0, GL_AMBIENT, no_light);
    }
    
    if (diff1 == true) {
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    } else {
        glLightfv(GL_LIGHT0, GL_DIFFUSE, no_light);
    }
    
    if (spec1 == true) {
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    } else {
        glLightfv(GL_LIGHT0, GL_SPECULAR, no_light);
    }
   
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); // Set light 1 position
    glPopMatrix(); // Restore previous matrix
}

void lightTwo()
{
    glPushMatrix(); // Save current matrix
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse[] = {1.0, 1.0, 0.9, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {0.0, 5.0, 8.0, 1.0}; // Set light 2 position
    
    // glEnable(GL_LIGHT1);
    
    // Set light properties based on switch state
    if (amb2 == true) {
        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    } else {
        glLightfv(GL_LIGHT1, GL_AMBIENT, no_light);
    }
    
    if (diff2 == true) {
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    } else {
        glLightfv(GL_LIGHT1, GL_DIFFUSE, no_light);
    }
    
    if (spec2 == true) {
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    } else {
        glLightfv(GL_LIGHT1, GL_SPECULAR, no_light);
    }
    
    glLightfv(GL_LIGHT1, GL_POSITION, light_position); // Set light 2 position
    glPopMatrix(); // Restore previous matrix
}

void lampLight()
{
    glPushMatrix(); // Save current matrix
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {0.7, 1.5, 9.0, 1.0}; // Set lamp light position
    
    // glEnable(GL_LIGHT2);
    
    // Set light properties based on switch state
    if (amb3 == true) {
        glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
    } else {
        glLightfv(GL_LIGHT2, GL_AMBIENT, no_light);
    }
    
    if (diff3 == true) {
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
    } else {
        glLightfv(GL_LIGHT2, GL_DIFFUSE, no_light);
    }
    
    if (spec3 == true) {
        glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
    } else {
        glLightfv(GL_LIGHT2, GL_SPECULAR, no_light);
    }

    glLightfv(GL_LIGHT2, GL_POSITION, light_position); // Set lamp light position
    
    GLfloat spot_direction[] = {0.3, -1, -0.8}; // Set spotlight direction
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 35.0); // Set spotlight cutoff angle
    glPopMatrix(); // Restore previous matrix
}




void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(60,1,1,100);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,eyeZ,  refX,refY,refZ,  0,1,0); //7,2,15, 0,0,0, 0,1,0
    
    glEnable(GL_LIGHTING);
    lightOne();
    lightTwo();
    lampLight();
    room();
    bed();
    bedsideDrawer();
    lamp();
    LinkinParkPoster();
    wallshelf();
    wardrobe();
    cupboard();
    dressingTable();
    Clock();
    window();
    sphericalObject();
    lightBulb1();
    lightBulb2();
    //lightBulb3();
    glDisable(GL_LIGHTING);
    
    glFlush();
    glutSwapBuffers();
}

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
        case 'w': // move eye point upwards along Y axis
            eyeY+=1.0;
            break;
        case 's': // move eye point downwards along Y axis
            eyeY-=1.0;
            break;
        case 'a': // move eye point left along X axis
            eyeX-=1.0;
            break;
        case 'd': // move eye point right along X axis
            eyeX+=1.0;
            break;
        case 'o':  //zoom out
            eyeZ+=1;
            break;
        case 'i': //zoom in
            eyeZ-=1;
            break;
        case 'q': //back to default eye point and ref point
                eyeX=7.0; eyeY=2.0; eyeZ=15.0;
                refX=0.0; refY=0.0; refZ=0.0;
            break;
        case 'j': // move ref point upwards along Y axis
            refY+=1.0;
            break;
        case 'n': // move ref point downwards along Y axis
            refY-=1.0;
            break;
        case 'b': // move ref point left along X axis
            refX-=1.0;
            break;
        case 'm': // move eye point right along X axis
            refX+=1.0;
            break;
        case 'k':  //move ref point away from screen/ along z axis
            refZ+=1;
            break;
        case 'l': //move ref point towards screen/ along z axis
            refZ-=1;
            break;
        case '1': //to turn on and off light one
            if(switchOne == false)
            {
                switchOne = true; amb1=true; diff1=true; spec1=true;
                glEnable( GL_LIGHT0); break;
            }
            else if(switchOne == true)
            {
                switchOne = false; amb1=false; diff1=false; spec1=false; glDisable( GL_LIGHT0); break;
            }
        case '2': //to turn on and off light two
            if(switchTwo == false)
            {
                switchTwo = true; amb2=true; diff2=true; spec2=true;
                glEnable( GL_LIGHT1); break;
            }
            else if(switchTwo == true)
            {
                switchTwo = false; amb2=false; diff2=false; spec2=false;
                glDisable( GL_LIGHT1); break;
            }
        case '3': //to turn on and off light one
            if(switchLamp == false)
            {
                switchLamp = true; amb3=true; diff3=true; spec3=true;
                glEnable( GL_LIGHT2); break;
            }
            else if(switchLamp == true)
            {
                switchLamp = false; amb3=false; diff3=false; spec3=false;
                glDisable( GL_LIGHT2); break;
            }
        case'4': //turn on/off ambient light 1
            if(amb1 == false) {amb1=true; break;}
            else{amb1=false; break;}
        case'5':
            if(diff1 == false) {diff1=true; break;}
            else{diff1=false; break;}
        case'6':
            if(spec1 == false) {spec1=true; break;}
            else{spec1=false; break;}
        case'7': //turn on/off ambient light 2
            if(amb2 == false) {amb2=true; break;}
            else{amb2=false; break;}
        case'8':
            if(diff2 == false) {diff2=true; break;}
            else{diff2=false; break;}
        case'9':
            if(spec2 == false) {spec2=true; break;}
            else{spec2=false; break;}
        case'e': //turn on/off ambient lamp light
            if(amb3 == false) {amb3=true; break;}
            else{amb3=false; break;}
        case'r':
            if(diff3 == false) {diff3=true; break;}
            else{diff3=false; break;}
        case't':
            if(spec3 == false) {spec3=true; break;}
            else{spec3=false; break;}
        case 27:    // Escape key
            exit(1);
    }
    
    glutPostRedisplay();
}


void animate()
{
    if(redFlag == true)
    {
        theta+=2;
        z-=0.02; //0.016667;
        if(theta >= 196 && theta <= 210)
        {
            y = 1.44;
        }
        else if(theta >= 180 && theta <= 194)
        {
            y = 1.42;
        }
        else if(theta >= 180 && theta <= 194)
        {
            y = 1.4;
        }
        else if(theta >= 164 && theta <= 178)
        {
            y = 1.42;
        }
        
        if(theta == 210)
        {
            redFlag = false;
        }
    }
    else if(redFlag == false)
    {
        theta-=2;
        z+=0.02;//0.016667;
        
        if(theta >= 196 && theta <= 210)
        {
            y = 1.44;
        }
        else if(theta >= 180 && theta <= 194)
        {
            y = 1.42;
        }
        else if(theta >= 180 && theta <= 194)
        {
            y = 1.4;
        }
        else if(theta >= 164 && theta <= 178)
        {
            y = 1.42;
        }
        
        if(theta == 150)
        {
            redFlag = true;
        }
    }
    
    glutPostRedisplay();

}

void fullScreen(int w, int h)
{
    //Prevent a divide by zero, when window is too short;you cant make a window of zero width.
    if (h == 0)
        h = 1;
    float ratio = (GLfloat)w / (GLfloat)h;         //Calculate aspect ratio of the window

    //Set the perspective coordinate system
    glMatrixMode(GL_PROJECTION);                   //Use the Projection Matrix
    glLoadIdentity();                              //Reset Matrix

    glViewport(0, 0, w, h);                        //Set the viewport to be the entire window
    gluPerspective(60, ratio, 1, 500);             //Set the correct perspective.
    //glFrustum(-2.5,2.5,-2.5,2.5, ratio, 200);
    glMatrixMode(GL_MODELVIEW);                    //Get Back to the Modelview
}


int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    
    std::cout<<"To move Eye point:"<< std::endl;
    std::cout<<"w: up"<<std::endl;
    std::cout<<"s: down"<<std::endl;
    std::cout<<"a: left"<<std::endl;
    std::cout<<"d: right"<<std::endl;
    std::cout<<"i: zoom in"<<std::endl;
    std::cout<<"o: zoom out"<<std::endl;
    std::cout<<"      "<<std::endl;
    std::cout<<"To move Camera point:"<< std::endl;
    std::cout<<"j: up"<<std::endl;
    std::cout<<"n: down"<<std::endl;
    std::cout<<"b: left"<<std::endl;
    std::cout<<"m: right"<<std::endl;
    std::cout<<"l: move nearer"<<std::endl;
    std::cout<<"k: move far"<<std::endl;
    std::cout<<"      "<<std::endl;
    std::cout<<"Press q to move to default position"<<std::endl;
    std::cout<<"      "<<std::endl;
    std::cout<<"For lighting:      "<<std::endl;
    std::cout<<"Light source 1 [the light on the right on the screen      "<<std::endl;
    std::cout<<"1: to turn on/off light one     "<<std::endl;
    std::cout<<"4: to turn on/off ambient light one     "<<std::endl;
    std::cout<<"5: to turn on/off diffusion light one     "<<std::endl;
    std::cout<<"6: to turn on/off specular light one      "<<std::endl;
    std::cout<<"      "<<std::endl;
    std::cout<<"Light source 2 [the light on the left on the screen "<<std::endl;
    std::cout<<"2: to turn on/off light two     "<<std::endl;
    std::cout<<"7: to turn on/off ambient light two     "<<std::endl;
    std::cout<<"8: to turn on/off diffusion light two     "<<std::endl;
    std::cout<<"9: to turn on/off specular light two      "<<std::endl;
    std::cout<<"      "<<std::endl;
    std::cout<<"Lamp light (spot light)" <<std::endl;
    std::cout<<"3: to turn on/off lamp     "<<std::endl;
    std::cout<<"e: to turn on/off ambient lamp light    "<<std::endl;
    std::cout<<"r: to turn on/off diffusion lamp light      "<<std::endl;
    std::cout<<"t: to turn on/off specular lamp light      "<<std::endl;
    std::cout<<"      "<<std::endl;
    std::cout<<"____________________"<<std::endl;
    std::cout<<"      "<<std::endl;
    std::cout<<"      "<<std::endl;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);

    glutInitWindowPosition(100,100);
    glutInitWindowSize(windowHeight, windowWidth);
    glutCreateWindow("1607063 Bedroom");

    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);
 
    glutReshapeFunc(fullScreen);
    glutDisplayFunc(display);
    glutKeyboardFunc(myKeyboardFunc);
    glutIdleFunc(animate);
    glutMainLoop();

    return 0;
}

