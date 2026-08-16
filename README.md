# Serialize (WIP)

Serialize is a serialization library for C++23 and, in the future, C++26.

Currently, most of the code is on the develop branch.

# Challenges

## Direct copies are error-prone

Although serialization might be seen as a simple task from the outside (transferring data from objects to files), it's a minefield where a single misstep can blow up your system.

Particularly in C++, serializing an object isn't a simple task because you need to watch out for three hazards of direct copies:

- Padding bytes might change between builds. While they aren't exactly random, a single update in the compiler's logic or an optimization flag might completely change the binary representation of a class, invalidating any previous byte-wise copies.
- If you serialize an object on a little-endian system and then deserialize it on a big-endian system, you'll corrupt your data. The little-endian-encoded data won't be automatically converted to big-endian, rendering the data useless on the destination system.
- Pointers are dangerous. You can't directly serialize pointers for two reasons:
- You need to serialize the pointed-to data as well.
	- The pointer's memory address shouldn't be serialized, because on deserialization there are no guarantees that the target address will still be available or point to a valid memory block of the correct size.

## Reflection

In order to solve the above problems, it's necessary to break down an unsafe class into its primitive components that are safe to directly copy.

However, to achieve that, you must build an entire reflection system through template metaprogramming that's able to recursively iterate through a class's fields (including private ones) and retrieve data about them.

This adds a massive layer of complexity to the library and forces the consuming code to include boilerplate to allow the library to work.

Part of this complexity should go away with C++26's reflection library, but contrary to what one might expect, part of the complexity will still remain—most notably, the recursive field inspection system.

## Plurality of formats

There's no universally accepted data format.

While JSON, XML, and binary are the most common formats, we shouldn't assume we know the consumer's needs better than they do.

Therefore, it's essential for the library to have an extensible formatting system that allows the consumer to define their own data formats.

## Modifying a class post-serialization

Even if serialization works correctly, changes to the fields of a class (like reordering, renaming, or data type changes) might break pre-existing serializations.

This necessitates a versioning system that allows for the graceful handling of chronological mismatches.

# Design choices

There are two ways to solve the major problems with serialization in C++:

- External reflection system (e.g., Google's FlatBuffers and Protobuf)
- Internal reflection system (e.g., Cereal, Boost.Serialization)

Like most design decisions in software engineering, the choice of which path to take boils down to trade-offs:

- An external reflection system (like an external compiler that generates the class code for the developer) adds build complexity, requires transitioning between C++ and an external file format, and can be awkward or impossible to implement for third-party data types. Yet, it's more scalable and requires no boilerplate.
- An internal reflection system (like one done through template metaprogramming) adds boilerplate code, can stretch compilation times, and is less flexible. However, it operates entirely inside C++ and can serialize third-party types if designed correctly.
- It's noteworthy that C++26's reflection system will change the dynamics between these trade-offs, allowing for cleaner, faster serialization systems.

Additionally, if you go with the internal reflection system, you must choose between intrusive and non-intrusive implementations. A good serialization library will allow the consumer to choose between the two, but non-intrusive implementations often beat intrusive ones in long-term flexibility.