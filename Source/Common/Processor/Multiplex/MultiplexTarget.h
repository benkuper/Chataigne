/*
  ==============================================================================

    MultiplexTarget.h
    Created: 10 Mar 2021 3:49:03pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Multiplex;

class MultiplexListener
{
public:
    virtual ~MultiplexListener() {}
    virtual void multiplexCountChanged() {}
    virtual void multiplexPreviewIndexChanged() {}
};


class MultiplexTarget :
    public MultiplexListener
{
public:
    MultiplexTarget(Multiplex* mp);
    virtual ~MultiplexTarget();

    Multiplex* multiplex;

    bool isMultiplexed() const;
    int getMultiplexCount() const;
    int getPreviewIndex() const;
};

class MultiplexListListener
{
public:
    virtual ~MultiplexListListener() {}
    virtual void listReferenceUpdated() {}
    virtual void listItemUpdated(int /*multiplexIndex */) {}
};
