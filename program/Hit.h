#pragma once

float GetDistance( float x1, float y1, float x2, float y2 );
bool CheckCircleHit( float x1, float y1, float r1, float x2, float y2, float r2 );
bool CheckPointCircleHit( float point_x, float point_y, float circle_x, float circle_y, float circle_r );
bool CheckBoxHit( float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2 );
bool CheckPointBoxHit( float point_x, float point_y, float box_x, float box_y, float box_w, float box_h );
