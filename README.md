# einfach-fliessend: a vocabulary utility programme

## The project
Improve your vocabulary using this simple utility programme, with an adaptive
algorithm that improves the learning efficiency by tuning the frequency of
questions. The app is flexible and offers many options to the user, including
how the algorithm should take into account various parameters in order to
adjust question frequencies, how words should appear depending on their gender,
and the details of what the data base should contain, as the user is responsible
for populating it.

## Design
An SQL data base is used in the background for storing words, their translations
and example sentences with their translations. We use the Qt library to provide
a simple interface and the project is fully implemented in C++.

## Potential extensions
A feature branch tries and packages the project under the debian standards, with
the main difficulty being to create the appropriate data base structure during
the installation process.

An obvious extension is to generalise the set of languages available for the
user to train his/her vocabulary.

Other ideas for possible extensions are listed in the "Issues" section of the
Github repository.
