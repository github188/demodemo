package MSFB::Schema::FBUser;
use base qw/DBIx::Class/;

__PACKAGE__->load_components(qw/Core/);
__PACKAGE__->table('ms_fbuser');
__PACKAGE__->add_columns(qw/usr_id uid, name/);
__PACKAGE__->set_primary_key('usr_id');

1;