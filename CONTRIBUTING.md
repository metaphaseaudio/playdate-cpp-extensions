# Contributing to PDCPP Extensions
Here is where you'll find the most up-to-date information on how to contribute
to this project. Contributions are highly encouraged: the scope of what's
possible with the Playdate using this library is limited only by the work of its
contributors.

## Code Style
This library closely follows the [JUCE coding standards](https://juce.com/coding-standards/),
with only minor alterations, so go read through that first.

The variations are:
* No whitespace before `()` and `<>` symbols for functions and templates
* Members are prefixed with a marker for to indicate the usage/provenance: `m_`
  for members, `p_` for pointers, `r_` for references, `k_` for constants, etc.
* Inheritance is listed vertically, and prefixed with the colon/comma
* Line lengths of 120 characters for code and code + inline comments
* Line lengths of 80 characters for block comments
* **Always** use `{}`s around `if` statements, no matter how short

If you find any further variations, or if something is unclear or inconsistent,
please fire off a question or PR. We're only human, after all, and mistakes do
happen. With time these variations will become fewer, and the style guide will
become clearer.

All new code must follow this style guide, and all new functions and classes
must come with DOXYGEN documentation to be considered for merging.

## Submitting your PR
Please at least skim Sebastien Castiel's [Pull Requests and Code Review](https://github.com/scastiel/book-pr/blob/main/manuscript.md).
It's a good book, and thorough, with lots of great recommendations.

In general, try to keep things constrained, and likewise a good faith effort
will be made to review things, or at least respond, in a timely fashion.
