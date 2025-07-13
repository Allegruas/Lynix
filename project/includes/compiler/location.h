#ifndef LOCATION_H
#define LOCATION_H

typedef struct Position {
	int line;
	int column;
}Position;

typedef struct Location {
	Position start;
	Position end;
}Location;

#endif // !LOCATION_H
