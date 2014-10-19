#ifndef INC_CHUNK_H
#define INC_CHUNK_H
#include <vector>

class Chunk
{
    int width_;
    int height_;
    int z_;
public:
    Chunk ( int width, int height, int z ) {
        width_ = width;
        height_ = height;
        z_ = z;
    }
    typedef std::vector<Chunk> ChunkVector;
    typedef std::vector<Chunk>::iterator ChunkIterator;
    Tile::TileVector vec_tiles_;
    int get_width () {
        return width_;
    }
    int get_height () {
        return height_;
    }
    int get_z () {
        return z_;
    }
    void CreateTile ( Tile tile ) {
        switch ( tile.get_type() ) {
        case GRASS:
            tile.set_tileset_coordinates(0, 0);
            break;
        case ROUGH:
            tile.set_tileset_coordinates(0, 32);
            break;
        case WATER:
            tile.set_tileset_coordinates(32, 0);
            tile.set_is_passable(false);
            break;
        case DIRT:
            tile.set_tileset_coordinates(64, 0);
            break;
        case FOG:
            tile.set_tileset_coordinates(288, 32);
            break;
        }
        vec_tiles_.push_back(tile);
    }
    Tile::TileIterator& ChangeTileType ( Tile::TileIterator &t ) {
        switch ( t->get_type() ) {
        case GRASS:
            t->set_tileset_coordinates(0, 0);
            break;
        case ROUGH:
            t->set_tileset_coordinates(0, 32);
            break;
        case WATER:
            t->set_tileset_coordinates(32, 0);
            break;
        case DIRT:
            t->set_tileset_coordinates(64, 0);
            break;
        case FOG:
            t->set_tileset_coordinates(288, 32);
            break;
        }
        //vec_tiles_.push_back(tile);
        return t;
    }
};

#endif
