#ifndef __YUNI_DBI_AUTO_COMMIT_ROLLBACK_H__
# define __YUNI_DBI_AUTO_COMMIT_ROLLBACK_H__



namespace Yuni
{
namespace DBI
{


	class AutoCommit final
	{
	public:
		explicit AutoCommit(ConnectorPool& connectors) :
			tx(std::move(connectors.begin()))
		{}

		AutoCommit(AutoCommit&& rhs) :
			tx(std::move(rhs.tx))
		{}

		~AutoCommit()
		{
			tx.commit();
		}

	public:
		//! The transaction
		Transaction tx;

	}; // class AutoCommit





} // namespace DBI
} // namespace Yuni

#endif // __YUNI_DBI_AUTO_COMMIT_ROLLBACK_H__
